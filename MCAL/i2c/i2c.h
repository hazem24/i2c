/*
 * i2c.h
 *
 * Created: 20-Jan-20 3:55:29 PM
 *  Author: LENOVO
 */ 


#ifndef I2C_H_
#define I2C_H_

/*- INCLUDES -----------------------------------------------*/
#include "../../common/compiler.h"
#include "i2c_config.h"

typedef uint8_t I2C_STATUS_t;
/*- CONSTANTS ----------------------------------------------*/
extern uint8_t au8_status;
/*- PRIMITIVE TYPES ----------------------------------------*/
/*- ENUMS --------------------------------------------------*/
/*- STRUCTS AND UNIONS -------------------------------------*/
typedef struct 
{
    uint32_t speed_mode;/*--> Bit rate of I2C Bus. <--*/
    uint8_t  prescaler;/* Prescaler of bit rate generator. */
}StrI2cConfig_t;

/*- FUNCTION-LIKE MACROS -----------------------------------*/
/*- FUNCTION DECLARATIONS ----------------------------------*/

#if MASTER_MODE == ENABLE
I2C_STATUS_t I2C_InitMaster(const StrI2cConfig_t* aStr_i2c_config);
/**
 * @brief Used to start a transimission.
 * @return I2C_STATUS_t 
 */
void I2C_StartBit();
/**
 * @brief Used to stop a transimission.
 * @return I2C_STATUS_t 
 */
void I2C_StopBit();
#endif


#if SLAVE_MODE == ENABLE
I2C_STATUS_t I2C_InitSlave();
#endif

I2C_STATUS_t I2C_SendByte(uint8_t au8_slave_address ,uint8_t au8_byte);

I2C_STATUS_t I2C_ReceiveByte(uint8_t* au8_byte);
#endif /* I2C_H_ */