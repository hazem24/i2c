/*
 * i2c_config.h
 *
 * Created: 20-Jan-20 3:55:46 PM
 *  Author: LENOVO
 */ 


#ifndef I2C_CONFIG_H_
#define I2C_CONFIG_H_

/*- INCLUDES -----------------------------------------------*/
#include "../../common/compiler.h"
/*- CONSTANTS ----------------------------------------------*/
#define ENABLE          TRUE
#define DISABLE         FALSE

#define MASTER_MODE     ENABLE
#define SLAVE_MODE      DISABLE

/* Speed of I2C Bus. */
#define STANDARD_MODE   100000UL
#define FAST_MODE       400000UL

/* Response type. */
#define INTERRUPT_MODE   ENABLE
#define POLLING_MODE     DISABLE

/* prescaler available. */
#define NO_PRESCALER        0U 
#define PRESCALER_4         1U
#define PRESCALER_16        2U
#define PRESCALER_64        3U


/* TWI STATUS. */
#define TWI_START_TRANSMITTED    (0x08U)
#define TWI_WRITE_ADDRESS_DONE   (0x20U)
#define TWI_DATA_TRANSMITTED     (0x30U)


#define TWI_SLA_W                (0x60U)
#define TWI_DATA_RECEIVED        (0x80U)
/* Address of this device in case of works as slave. */
#define SLAVE_ADDRESS 0x0A



#define TWI_SUCCESS              (50U)
/*- PRIMITIVE TYPES ----------------------------------------*/
/*- ENUMS --------------------------------------------------*/
/*- STRUCTS AND UNIONS -------------------------------------*/
/*- FUNCTION-LIKE MACROS -----------------------------------*/
/*- FUNCTION DECLARATIONS ----------------------------------*/

#endif /* I2C_CONFIG_H_ */