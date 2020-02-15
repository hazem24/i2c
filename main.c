/*
 * i2c_lib.c
 *
 * Created: 20-Jan-20 3:53:36 PM
 * Author : LENOVO
 */ 


#include "MCAL/i2c/i2c.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include "micro_registers.h"

int main(void)
{
    /* Replace with your application code */
    StrI2cConfig_t config = {FAST_MODE, NO_PRESCALER};
    I2C_InitMaster(&config);

    I2C_SendByte(0b00001010, 0xAA);
    while (1) 
    {
        
        PORTA = au8_status;
    }
}

