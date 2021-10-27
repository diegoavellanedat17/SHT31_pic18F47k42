/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F47K42
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <stdio.h>


static i2c1_operations_t wr2RegCompleteHandler(void *ptr);
static i2c1_operations_t rd2RegCompleteHandler(void *ptr);
bool I2C1_MasterIsNack(void);


void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    //TRISC = 0xBF;
    //TRISC = 0xFF;
    //ODCOND = 0x00;
    int temp;
    float temperature_celsius;
    while (1)
    {
        __delay_ms(2000);
        //datos para hacer que realice una medida 0x2c mas significativo 0x06 LS
        uint16_t data = 0x62C;
        uint8_t dataL = 0x2C;
        uint16_t returnValue =0x00;

        
            while(!I2C1_Open(0x44)); // sit here until we get the bus..
                // Ya que tenemos la comunicación I2C abierta vamos a enviar in control write
                I2C1_SetBuffer(&data,2);
                I2C1_MasterWrite();
                // se envia la direccion con los datos 
                __delay_ms(100);
                 I2C1_SetDataCompleteCallback(rd2RegCompleteHandler,&returnValue);
                 I2C1_SetBuffer(&dataL,1);
                I2C1_SetAddressNackCallback(NULL,NULL); //NACK polling?
                I2C1_MasterWrite();
                //I2C1_MasterRead();
                //I2C1_SetDataCompleteCallback(wr2RegCompleteHandler,&returnValue);
                //I2C1_SetAddressNackCallback(NULL,NULL); //NACK polling?
                
      
                temp = returnValue << 8 | returnValue >> 8;

               
                temperature_celsius =(-45.0 + (175.0 * temp / 65535.0));
                printf("Temperatura = %f °C \n",temperature_celsius);
            while(I2C1_BUSY == I2C1_Close()); // sit here until finished.
    }
}
/**
 End of File
*/

static i2c1_operations_t wr2RegCompleteHandler(void *ptr)
{
    I2C1_SetBuffer(ptr,2);
    I2C1_SetDataCompleteCallback(NULL,NULL);
    return I2C1_CONTINUE;
}

bool I2C1_MasterIsNack(void)
{
    return I2C1CON1bits.ACKSTAT;
}
static i2c1_operations_t rd2RegCompleteHandler(void *ptr)
{
    I2C1_SetBuffer(ptr,2);
    I2C1_SetDataCompleteCallback(NULL,NULL);
    return I2C1_RESTART_READ;
}