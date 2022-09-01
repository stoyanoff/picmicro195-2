/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  PIC24FJ64GA002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/
#ifndef FCY
#define FCY  29491200UL
#endif
/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/serial1.h"
#include "mcc_generated_files/traps.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "libpic30.h"
#include "string.h"


//Manual functions

    uint8_t incoming_seq[7];
    int count = 0;
    int counter = 0;
    char temp;
//uart transmit entire string function
void UART1TransmitBytes (uint8_t *tx_str){
    uint8_t ch;
    while (*tx_str!=0x00) {
        ch=*tx_str;
        UART1_Write(ch);
        tx_str++;
    }
}
//Redefine callback function for UART1 receive interrupt
void UART1_Receive_CallBack(void){
    count=0;
    while (UART1_IsRxReady()){
        incoming_seq[count]=UART1_Read();
        count++;
    }
}


/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    SERIAL1_Initialize();
    INTERRUPT_Initialize();
    INTERRUPT_GlobalEnable();
    
    UART1TransmitBytes("Start debug session\r\n");
    __delay_ms(500);
    

    
    
    
    while (1) {

        
        
        
        
//        do {
//        incoming_seq[count]=UART1_Read();
//        }
//        while (UART1_RX_DATA_AVAILABLE==1);

//    count = UART1_ReceiveBufferSizeGet();
//    if (UART1_IsRxReady())
//    {
//        count = UART1_ReceiveBufferSizeGet();
//        incoming_seq[count]=UART1_Read();
//        counter++;
//    }
//    if (count>0) {
//    UART1TransmitBytes('Receiver OK');
//    }
//    {
//        if ((incoming_seq[0]==0x61)&&(incoming_seq[1]==0x61)){
//            UART1_Write(0x4F);
//            UART1_Write(0x4B);
//            UART1_Write(0x0A);
//            UART1_Write(0x0D);
//        }
//        
//    }
        
}
        
    return 0;
}
/**
 End of File
*/

