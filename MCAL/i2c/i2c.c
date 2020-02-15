/*
 * i2c.c
 *
 * Created: 20-Jan-20 3:55:19 PM
 *  Author: LENOVO
 */ 
/*- INCLUDES ----------------------------------------------*/
#include "i2c.h"
#include "../../micro_config.h"
#include "../../micro_registers.h"
#include "../../common/macros.h"
#include <math.h>

/*- LOCAL MACROS ------------------------------------------*/
#define CLEAR_STATUS  (0x07)
#define STATUS_READER (0xF8)


#define IDLE_MODE                       (0U)
#define START_DONE                      (1U)
#define ADDRESS_WRITE_DONE              (2U)
#define ADDRESS_READ_DONE               (3U)
#define SEND_BYTE                       (4U)
#define STOP_BIT                        (5U)

#define WAITING_DATA                    (6U)
#define DATA_RECEIVED                   (7U)

#define TWI_WAIT_RESPONSE               (20U)
#define TWI_ADDRESS_WRITTING            (21U)
/*- LOCAL Dataypes ----------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
static uint8_t gu8_TWI_state_machine = IDLE_MODE;/* Used to indicate where twi stopped at. */
static uint8_t gu8_last_byte_received;/* Saved Last Byte received. */
static uint8_t gu8_address_byte_received;
uint8_t au8_status;
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

#if MASTER_MODE == ENABLE
I2C_STATUS_t I2C_InitMaster(const StrI2cConfig_t* aStr_i2c_config)
{
    /* TWBR => Master. --DONE. */
    TWBR  = (uint8_t)((F_CPU/(aStr_i2c_config->speed_mode)) - 16U)/(2 * pow(4, aStr_i2c_config->prescaler));
    TWSR |= aStr_i2c_config->prescaler;
    /**
        * TWCR
            * TWIE:  TWI Interrupt Enable --DONE.
            * TWEN:  TWI Enable Bit --DONE.
            * TWSTO: TWI STOP Condition Bit  => in case of Master.--DONE.
            * TWSTA: TWI START Condition Bit => in case of Master.--DONE.
            * TWEA:  TWI Enable Acknowledge Bit => Enable ACK.--DONE.
            * TWINT: TWI Interrupt Flag.
        * TWSR
            * Bits [7:3] – TWS: TWI Status.
            * Bits [1:0] – TWPS: TWI Prescaler Bits. --DONE.
            * Prescaler should be masked when checking for status of I2C. 
        * TWI (Slave) Address Register – TWAR. --DONE.
            * Bits 7..1 – TWA: TWI (Slave) Address Register.
    */
#if INTERRUPT_MODE == ENABLE
    SET_BIT(TWCR, TWIE);/* Enable Interrupt. */
    sei();
#endif
    //SET_BIT(TWCR, TWEA);/* Enable ACK. */
    /* Set Address. */
    TWAR = SLAVE_ADDRESS << 1;
    SET_BIT(TWCR, TWEN);/* Enable TWI Module. */

    return TWI_SUCCESS;
}

/**
 * @brief Used to start a transimission.
 * @return I2C_STATUS_t 
 */
void I2C_StartBit()
{
    /**
     * TWCR:
        * TWSTO: TWI STOP Condition Bit  => in case of Master.
        * TWSTA: must be cleared by software when the START condition has been transmitted.
     */
    /* TWSTA must be cleared by software when the START condition has been transmitted. */
    
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN) | (1 << TWIE);
}
/**
 * @brief Used to stop a transimission.
 * @return I2C_STATUS_t 
 */
void I2C_StopBit()
{
    TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO) | (1 << TWIE);
}
#endif
 

#if SLAVE_MODE == ENABLE
I2C_STATUS_t I2C_InitSlave()
{
    /**
        * TWCR
            * TWIE: TWI Interrupt Enable 
            * TWEN: TWI Enable Bit
            * TWSTO: TWI STOP Condition Bit    => in case of Master.
            * TWSTA: TWI START Condition Bit   => in case of Master.
            * TWEA: TWI Enable Acknowledge Bit => Enable ACK.
            * TWINT: TWI Interrupt Flag.
        * TWSR
            * Bits [7:3] – TWS: TWI Status.
            * Bits [1:0] – TWPS: TWI Prescaler Bits.
            * Prescaler should be masked when checking for status of I2C. 
        * TWI (Slave) Address Register – TWAR.
            * Bits 7..1 – TWA: TWI (Slave) Address Register.
    */

    #if INTERRUPT_MODE == ENABLE
        SET_BIT(TWCR, TWIE);/* Enable Interrupt. */
        sei();
    #endif

    SET_BIT(TWCR, TWEA);/* Enable ACK. */
    /* Set Address. */
    TWAR = SLAVE_ADDRESS << 1;
    SET_BIT(TWCR, TWEN);/* Enable TWI Module. */
    return TWI_SUCCESS;
}
#endif

I2C_STATUS_t I2C_SendByte(uint8_t au8_slave_address ,uint8_t au8_byte)
{
    /**
    * TWI Data Register – TWDR.
    */
   gu8_address_byte_received = au8_slave_address;
   gu8_last_byte_received    = au8_byte;

   TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN) | (1 << TWIE);

   return TWI_SUCCESS;    
}

I2C_STATUS_t I2C_ReceiveByte(uint8_t* au8_byte)
{
    /**
     * TWI Data Register – TWDR. 
    */
   switch (gu8_TWI_state_machine)
   {
   case DATA_RECEIVED:
       if ( NULL != au8_byte )
       {
           *au8_byte = gu8_last_byte_received;
       }
       else
       {
           /* NOP. */
       }
       gu8_TWI_state_machine = IDLE_MODE;
       break;
   default:
       break;
   }

   return TWI_SUCCESS;
}


ISR( TWI_vect )
{
    /* Read Status. */
    au8_status = ((STATUS_READER & (TWSR)));
#if MASTER_MODE == ENABLE
    if( gu8_TWI_state_machine == IDLE_MODE )
    {
        TWDR = gu8_address_byte_received; 
        TWCR = (1<<TWINT) | (1<<TWEN)| (1 << TWIE);
        gu8_TWI_state_machine = ADDRESS_WRITE_DONE;
    }
    else if ( gu8_TWI_state_machine == ADDRESS_WRITE_DONE )
    {
        /* SLA + W. */
        TWDR = gu8_last_byte_received;
        //Clear Flag.
        TWCR = (1<<TWINT) | (1<<TWEN)| (1 << TWIE);
        gu8_TWI_state_machine = SEND_BYTE;
    }
    else if ( gu8_TWI_state_machine == SEND_BYTE )
    {
        /* Send ADDRESS With Write. */
        I2C_StopBit(); 
        gu8_TWI_state_machine = IDLE_MODE;
    }
    else
    {
        gu8_TWI_state_machine = IDLE_MODE;
    }
#endif

#if SLAVE_MODE == ENABLE
    if( au8_status == TWI_SLA_W )
    {
        gu8_TWI_state_machine = WAITING_DATA;
    }
    else if ( au8_status == TWI_DATA_RECEIVED )
    {
        gu8_TWI_state_machine  = DATA_RECEIVED;
    }
    else
    {
        gu8_TWI_state_machine = IDLE_MODE;
    }
    
#endif
}