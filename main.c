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
//#ifndef PLLMODE
//#define PLLMODE      4               		//On-chip PLL setting
//#endif

#ifndef FCY
#define FCY 14745600UL
//3686400UL//29491200UL
#endif

//#define BAUDRATE         9600       
//#define BRGVAL          ((FCY/BAUDRATE)/16)-1 

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/serial1.h"
#include "mcc_generated_files/traps.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/pin_manager.h"
#include "libpic30.h"
#include "string.h"


//Manual functions
//these comming from pc
    uint8_t incoming_seq[20];
    uint8_t asic_command[9];
//variables for the timer1    
//    bool statusTimer1;
//    uint16_t period;
//    uint16_t value;
//sequencing income commands    
    uint8_t read_seq[2]={0x61, 0x61};
    uint8_t write_seq[2]={0x06, 0x01};
//    uint8_t tmr1_count = 0;
//    uint8_t bingo_array[6]=    {0x42, 0x69, 0x6e, 0x67, 0x6f, 0x00};
//    int res = 0;
//    int buff = 0;
    uint8_t count = 0;
//    char temp;
//    char Rx1buffer;

//Timer1 Interrupt CallBack function    
//    void TMR1_CallBack(void){
//        tmr1_count++;
//    }
    
//uart transmit entire string function
void UART1TransmitBytes (uint8_t *tx_str){
    uint8_t ch;
    while (*tx_str!=0x00) {
        ch=*tx_str;
        UART1_Write(ch);
        tx_str++;
    }
}

//Enter specific test mode, number ot the mode as parameter
//IO8/RB7 need to provide 7 pulses with length of 100uS each
void ASIC_EnterTestMode(uint8_t mode){
    SW1_SetHigh();//TEST2 to VDD
    __delay_us(2);
    for (int i=1; i<=mode; i++){
        TEST5_SetHigh();
        __delay_us(100);   //min PW3 = 100uS
        TEST5_SetLow();
        __delay_us(20);    //min T2 = 120uS
        }

//    return 0;
}
//Redefine callback function for UART1 receive interrupt
void UART1_Receive_CallBack(void){
    char ch;
    _U1RXIF = 0;
    ch = U1RXREG;
    incoming_seq[count++] = ch;
    if (count > 20)count = 0;

}


//Function to serial read parameters from ASIC via serial
//After entering test mode 7, IO6RB3 need to provide clock with period min 20uS
//and pulse width of 10uS, while IO10/RB11 need to provide pulses for each 1 in the bytes    
void ASIC_SerialRead(void){
    ASIC_VDD_SetHigh();//Power up the ASIC
    __delay_ms(25);
    VBST_SetHigh();
    __delay_ms(25);
    ASIC_EnterTestMode(7); //Call test mode 7
    __delay_us(100);
    for (int c=1; c<=6;c++){
        for (int i=1; i<=8;i++){
    //RB3 is FEED        
            FEED_SetHigh();
            __delay_us(17);
            FEED_SetLow();
            __delay_us(20);
            }
        __delay_ms(1);
    }
    ASIC_VDD_SetLow(); //Power off the ASIC
    SW1_SetLow();//TEST2 to VSS
}

void ASIC_SerialWrite(void){
//Function to serial write parameters to ASIC via serial
}


/*
                         Main application
 */
int main(void)
{   
    // initialize the device
    SYSTEM_Initialize();
    SERIAL1_Initialize();
    INTERRUPT_GlobalEnable();
    SW1_SetLow();
    FEED_SetLow();
//    ASIC_VDD_SetHigh(); //Power up the ASIC
//  Endless loop
    while (1) {
//      PORTBbits.RB15 ^=1;  //Just for testing - toggle LED4
        __delay_ms(30);
//      Begin handling asic command
        if (count == 2){
//      Incoming sequence array contains command from PC software and data for ASIC
//      Copy incoming sequence to asic_command array for further manipulation  
            memcpy(asic_command,incoming_seq, sizeof(asic_command));            
            if ((asic_command[0]==0x61) && (asic_command[1]==0x61)){
                ASIC_SerialRead();
            }
            if ((asic_command[0]==0x06) && (asic_command[1]==0x01)){
            ASIC_SerialWrite();
            }
            
        }
        count=0;
//      Empty incoming sequence array for next command                    
            memset(incoming_seq,0,sizeof(incoming_seq));

        ASIC_VDD_SetLow();

    }; //end of Loop
}; //End of main
    /**
 End of File
*/

