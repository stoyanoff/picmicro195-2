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
    uint8_t incoming_seq[9];
    uint8_t asic_command[9];
    uint8_t read_seq[2]={0x61, 0x61};
    uint8_t write_seq[2]={0x06, 0x01};
    uint8_t count = 0;
    uint8_t bingo_array[6]=    {0x42, 0x69, 0x6e, 0x67, 0x6f, 0x00};
    int res = 0;
    int buff = 0;
    int counter = 0;
    char temp;
    char Rx1buffer;
    
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
//void UART1_Receive_CallBack(void){
//    do {
//       incoming_seq[count]=UART1_Read();
//        count++;
//    } while (UART1_IsRxReady());
//    

//}

void ASIC_SerialRead(void){
    //int i=0;
//    for (int i=0; sizeof(bingo_array); i++){
//        UART1_Write(bingo_array[i]);
    UART1TransmitBytes(bingo_array);
//    }
    return 0;
}

void ASIC_SerialWrite(void){
    //int i=0;
//    for (int i=0; sizeof(bingo_array); i++){
//        UART1_Write(bingo_array[i]);
    UART1_Write(0x48);
//    }
    return 0;
}


/*
                         Main application
 */
int main(void)
{   
    // initialize the device
    SYSTEM_Initialize();
    SERIAL1_Initialize();
    
 //   INTERRUPT_GlobalEnable();
//  Endless loop
    while (1) {
//      __delay_ms(5000);
        count=0;
//      Reading UART if is is not empty  
        if (!UART1_ReceiveBufferIsEmpty()) {
            //if (UART1_IsRxReady()) {
                buff = UART1_ReceiveBufferSizeGet();
                for (count=0; count<10; count++){
                    incoming_seq[count]=UART1_Read();
                }
            //} 
        }
//      Incoming sequence array contains command from PC software
//      Copy incoming sequance to asic_command array for further manipulation  
        memcpy(asic_command,incoming_seq, sizeof(asic_command));
//      Empty incoming sequence array for next command        
//        memset(incoming_seq,0,sizeof(incoming_seq));
        __delay_ms(1500);
        if (asic_command[0]!=0) {
//      Begin handling asic command
        res = memcmp(asic_command, read_seq,2);
        if (res ==0){
             ASIC_SerialRead();
        }
        res = memcmp(asic_command, write_seq,2);
        if (res == 0){
             ASIC_SerialWrite();
        }        
//        if ((asic_command[0]==0x61)&&(asic_command[1]==0x61)) {
//            ASIC_SerialRead();
//        }
//        if ((asic_command[0]==0x06)&&(asic_command[1]==0x01)) {
//            ASIC_SerialWrite();    
//        }
        
        
        
        
        }
        memset(asic_command,0,sizeof(asic_command));
    }
    return 0;
}
/**
 End of File
*/

