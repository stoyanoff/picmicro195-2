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
#include "xc.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/serial1.h"
#include "mcc_generated_files/traps.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/pin_manager.h"
#include "libpic30.h"
#include "stdlib.h"
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
    uint8_t data_to_write[6];
//    uint8_t tmr1_count = 0;
//    uint8_t bingo_array[6]=    {0x42, 0x69, 0x6e, 0x67, 0x6f, 0x00};
//    int res = 0;
//    int buff = 0;
    uint8_t count = 0;
    char temp[8];
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

void ASIC_PowerUp() {
    ASIC_VDD_SetHigh();//Power up the ASIC
    __delay_ms(25);  //wait to stabilize Vdd
    VBST_SetHigh();  //5V to pin Vbst
    __delay_ms(25);  //wait to stabilize Vbst 
    IRCAP_SetHigh();  //5V to pin IRCAP
    SW1_SetHigh();//TEST2 to VDD
    __delay_us(5);    
}

void ASIC_PowerDown() {
    ASIC_VDD_SetLow(); //Power off the ASIC
    VBST_SetLow(); //Turn off Vbst
    SW1_SetLow();//TEST2 to VSS
    IRCAP_SetLow();  //turn off IRCAP     
}

//Enter specific test mode, number of the mode as parameter
//IO8/RB7 need to provide 7 pulses with length of 100uS each
void ASIC_EnterTestMode(uint8_t mode){
    ASIC_PowerUp();
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
   

void ASIC_StartCalibration(void){
    uint8_t HB_Status=0;
    uint8_t integrator_level=0;
    for (int test_mode=3; test_mode<=6; test_mode++) {
        if (test_mode==3){
            ASIC_EnterTestMode(test_mode);
        }//end if
        else {
            TEST5_SetHigh();  //Next calibration mode, page 21 p.5, 6, 7
            __delay_us(100);   //min PW3 = 100uS
            TEST5_SetLow();
            __delay_us(20);    //min T2 = 120uS            
        }//end else
        
        __delay_ms(2);  //Wait to pass transition process on
        
        /*For Testing! only!*/
//        while (integrator_level<=63){
//        FEED_SetHigh();
//        __delay_ms(1);
////        IO_SetHigh();
////        __delay_ms(2);
////        IO_SetLow();
////        __delay_ms(2);
//        FEED_SetLow();
//        __delay_ms(1);
//        
//        integrator_level++;
//        }


        // Like datasheet diagram        
//         __delay_ms(2);
//        FEED_SetHigh();
//        IO_SetHigh();
//        __delay_ms(2);
//        IO_SetLow();
//        __delay_ms(1);
//        IO_SetHigh();
//        __delay_ms(2);
//        IO_SetLow();
//        FEED_SetLow();
//        __delay_ms(1);
        
        while (integrator_level<=63) {
            
            FEED_SetHigh();         //Increase GAMP
            IO_SetHigh();            //Start measurement
            __delay_ms(2);
            IO_SetLow();            
            FEED_SetLow();
            HB_Status=HB_GetValue();    //Get status of HB pin


            __delay_ms(1);
            
            IO_SetHigh();            //Latch last GAMP value
            __delay_ms(2);
            IO_SetLow();            
            
            if (HB_Status==0){      //high if Gamp < IntegOut; low if Gamp > IntegOut
                HB_Status=0;
                break;}             //If measured level is lower than comparator level, exit the loop
            
            __delay_ms(1);            

            integrator_level++;
//            __delay_ms(1);
        } //end while
        integrator_level=0;
   
    
        IO_SetHigh();               //After T6 save latched values to EEPROM
        __delay_ms(12);
        IO_SetLow();
        
    

    } //end for
    UART1_Write(0x55);              //Read the values, saved in ASIC's EEPROM
}//end function


//Function to serial read parameters from ASIC via serial
//After entering test mode 7, IO6RB3 need to provide clock with period min 20uS
//and pulse width of 10uS, while IO10/RB11 need to provide pulses for each 1 in the bytes    
void ASIC_SerialRead(void){
    uint8_t HB_SerialOut_status;
    int dec_value;
    int base_value = 1;
    ASIC_EnterTestMode(7); //Call test mode 7
    __delay_us(100); //as per datasheet
    UART1_Write(0x44);//Response to the PC always start with 0x44
    for (int c=1; c<=6;c++){ //ASIC's config word is 44 bits, so nearest byte count is 6
        for (int i=1; i<=8;i++){
            //need to pulse FEED pin 8 times to collect 8 bits to form 1 byte
            //RB3 is FEED        
            FEED_SetHigh();
            HB_SerialOut_status=HB_GetValue(); //Get value on pin HB
            __delay_us(17);
            FEED_SetLow();
            __delay_us(20);
            dec_value +=base_value*HB_SerialOut_status; //converting binary to decimal value
            base_value=base_value*2; //2^î
        }
        UART1_Write(dec_value);  //Send current byte
        //start over for next byte
        dec_value=0;
        base_value = 1; //2^0=1
        while (U1STAbits.TRMT==0) { //but wait until current byte is sent
            Nop();
        }
}       
    ASIC_PowerDown();
    }


void ASIC_SerialWrite(void){
//Function to serial write parameters to ASIC via serial
    uint8_t buffer_count = 0; //ASIC shift register buffer size is 44 bit max
    ASIC_EnterTestMode(7); //Call test mode 7
    __delay_us(100); //as per datasheet
    for (int byte_number=2; byte_number<=7; byte_number++){  //Get byte from the array with data
        for (int BitNumber=7; BitNumber>=0; BitNumber-- ){  //Get each single bit of the current byte
//            FEED_SetHigh();
            if (((asic_command[byte_number]>>BitNumber) & 1) && (buffer_count<=43)) {  //if bit is 1, TEST data set to HIGH
                TEST3_3_SetHigh();
                __delay_us(5);
                FEED_SetHigh();
                __delay_us(20);
                FEED_SetLow();                
                __delay_us(5);               
            }
            else if (buffer_count>43){  //else TEST data is 0
                TEST3_3_SetLow();
                __delay_us(5);
                FEED_SetHigh();
                __delay_us(20);
                FEED_SetLow();                
                __delay_us(5);                 
            }
//            __delay_us(17);
//            FEED_SetLow();
//            __delay_us(20);            
            buffer_count++;  //need to save 44 bits, but 6 bytes are 48, so we are stop writing at bit 44 
        }
    }
    __delay_us(300);
    IO_SetHigh();   //Latch data to ASIC's memory
    __delay_ms(40);
    IO_SetLow();
    UART1_Write(0x4b);  //Let PC knows data are saved
    
    ASIC_PowerDown();
}

void ASIC_T2UserSelections (void) {
    //Function to write user selections parameters to ASIC via mode T2 (serial, 14 bit))
    uint8_t buffer_count = 0; //ASIC T2 mode shift register buffer size is 14 bit max
    ASIC_EnterTestMode(2); //Call test mode 7
    __delay_us(100); //as per datasheet
    
    for (int byte_number=3; byte_number<=4; byte_number++){  //Get byte from the array with data
               for (int BitNumber=7; BitNumber>=0; BitNumber-- ){  //Get each single bit of the current byte
//            FEED_SetHigh();
            if (((asic_command[byte_number]>>BitNumber) & 1) && (buffer_count<=13)) {  //if bit is 1, TEST data set to HIGH
                TEST3_3_SetHigh();
                __delay_us(5);
                FEED_SetHigh();
                __delay_us(20);
                FEED_SetLow();                
                __delay_us(5);               
            }
            else if ((buffer_count<=13)){  //else TEST data is 0
                TEST3_3_SetLow();
                __delay_us(5);
                FEED_SetHigh();
                __delay_us(20);
                FEED_SetLow();                
                __delay_us(5);                 
            }
//            __delay_us(17);
//            FEED_SetLow();
//            __delay_us(20);            
            buffer_count++;  //need to save 44 bits, but 6 bytes are 48, so we are stop writing at bit 44 
        }
    }
    __delay_us(300);
    IO_SetHigh();   //Latch data to ASIC's memory
    __delay_ms(20);
    IO_SetLow();
    UART1_Write(0x44);  //Let PC knows data are saved
    
    ASIC_PowerDown();    
}

void ASIC_ChamberMonitorLimitSet(void){  //Mode T8
    uint8_t HB_pin_status = 0;    
    ASIC_EnterTestMode(8); //Call test mode T8
    __delay_us(10); //as per datasheet page 24 figure 4-6
    FEED_SetHigh();
    __delay_ms(3);
    FEED_SetLow();
    __delay_ms(100);
    UART1_Write(0x44);
    IO_SetHigh();
    __delay_ms(40);
    IO_SetLow();
//    HB_pin_status = HB_GetValue();  //read HB, if 0 no alarm
//    __delay_us(100);
//    if (!HB_pin_status){
//    }

    ASIC_SerialRead();    
    __delay_ms(33);    
    ASIC_PowerDown();
}

void ASIC_LimitsCheck(uint8_t test_mode){  //Modes T12, T11, T10, T9
    uint8_t HB_pin_status = 0;    
    ASIC_EnterTestMode(test_mode); //Call test mode depends of PC software command]
    __delay_us(120); //as per datasheet
    FEED_SetHigh();
    __delay_ms(2);
    FEED_SetLow();
    HB_pin_status = HB_GetValue();  //read HB, if 0 no alarm
    __delay_us(100);
    if (!HB_pin_status){
        UART1_Write(0x4e);
        }
    else {
        UART1_Write(0x41);
    }
    ASIC_PowerDown();    
}

//void ASIC_NormalLimCheck(void){ //Mode T9
//    uint8_t HB_pin_status = 0;    
//    ASIC_EnterTestMode(9); //Call test mode T9
//    __delay_us(120); //as per datasheet
//    FEED_SetHigh();
//    __delay_ms(2);
//    FEED_SetLow();
//    HB_pin_status = HB_GetValue();  //read HB, if 1 no alarm
//    __delay_us(100);
//    if (!HB_pin_status){
//        UART1_Write(0x4e);
//    }
//    ASIC_PowerDown();    
//}
//
//void ASIC_HystLimCheck(void){  //Mode T10
//    uint8_t HB_pin_status = 0;    
//    ASIC_EnterTestMode(10); //Call test mode T10
//    __delay_us(120); //as per datasheet
//    FEED_SetHigh();
//    __delay_ms(2);
//    FEED_SetLow();
//    HB_pin_status = HB_GetValue();  //read HB, if 1 no alarm
//    __delay_us(100); 
//    if (!HB_pin_status){
//        UART1_Write(0x4e);
//    }
//    ASIC_PowerDown();    
//}
//
//void ASIC_HushLimCheck (void) {  //Mode T11
//    uint8_t HB_pin_status = 0;    
//    ASIC_EnterTestMode(11); //Call test mode T11
//    __delay_us(120); //as per datasheet
//    FEED_SetHigh();
//    __delay_ms(2);
//    FEED_SetLow();
//    HB_pin_status = HB_GetValue();  //read HB, if 1 no alarm
//    __delay_us(100);
//    
//    if (!HB_pin_status){
//        UART1_Write(0x4e);
//    }
//    ASIC_PowerDown();
//}
//
//void ASIC_ChamberTestLimitsCheck(void){  //Mode T12
//    uint8_t HB_pin_status = 0;  
//    ASIC_EnterTestMode(12); //Call test mode T12
//    __delay_us(120); //as per datasheet
//    FEED_SetHigh();
//    __delay_ms(2);
//    FEED_SetLow();
//    HB_pin_status = HB_GetValue();  //read HB, if 1 no alarm
//    __delay_us(100);
//    
//    ASIC_VDD_SetLow(); //Power off the ASIC
//    VBST_SetLow(); //Turn off Vbst
//    SW1_SetLow();//TEST2 to VSS
//    IRCAP_SetLow();  //turn off IRCAP 
//    if (!HB_pin_status){
//        UART1_Write(0x4e);
//    }
//    ASIC_PowerDown();
//}
///*
//                         Main application
// */
void main(void)
{   
    // initialize the device
    SYSTEM_Initialize();
    INTERRUPT_GlobalEnable();
    SERIAL1_Initialize();

    SW1_SetLow();  //Pin TEST2 set low
    FEED_SetLow(); //Pin FEED set low
//  Endless loop
    while (1) {
//      PORTBbits.RB15 ^=1;  //Just for testing - toggle LED4
        __delay_ms(30);
//      Begin handling asic command
        if ((count == 2)||(count==3)||(count==5)||(count==8)){  //2 bytes command for read, 2+6 bytes command and data to write
//      Incoming sequence array contains command from PC software and data for ASIC
//      Copy incoming sequence to asic_command array for further manipulation  
            memcpy(asic_command,incoming_seq, sizeof(asic_command));            
            if ((asic_command[0]==0x61) && (asic_command[1]==0x61)){
                ASIC_SerialRead();
            }
            if ((asic_command[0]==0x06) && (asic_command[1]==0x01)){
                for (int i=2; i<=8;i++ ){
                    data_to_write[i-2]=asic_command[i];
                }
                ASIC_SerialWrite();
            }
            
        }
        if (count == 3){  //3 bytes commands for limits check
            memcpy(asic_command,incoming_seq, sizeof(asic_command));
            if ((asic_command[0]==0x54)&&(asic_command[1]==0x4d)&&(asic_command[2]==0x43)){
                ASIC_LimitsCheck(12);
//                ASIC_ChamberTestLimitsCheck();
            }
            if ((asic_command[0]==0x54)&&(asic_command[1]==0x4d)&&(asic_command[2]==0x42)){
                ASIC_LimitsCheck(11);
//                ASIC_HushLimCheck();
            }
            if ((asic_command[0]==0x54)&&(asic_command[1]==0x4d)&&(asic_command[2]==0x41)){
                ASIC_LimitsCheck(10);
//                ASIC_HystLimCheck();
            }
            if ((asic_command[0]==0x54)&&(asic_command[1]==0x4d)&&(asic_command[2]==0x39)){
                ASIC_LimitsCheck(9);
//                ASIC_NormalLimCheck();
            }
            if ((asic_command[0]==0x54)&&(asic_command[1]==0x4d)&&(asic_command[2]==0x36)){
                ASIC_StartCalibration();
//                ASIC_NormalLimCheck();
            }            
        }
        if (count==5){  //5 bytes command for chamber monitor limit set
            if ((asic_command[0]==0x54)&&(asic_command[1]==0x4d)&&(asic_command[2]==0x38)&&(asic_command[3]==0x61)&&(asic_command[4]==0x61)){
                ASIC_ChamberMonitorLimitSet();
            }
            if ((asic_command[0]==0x54)&&(asic_command[1]==0x4d)&&(asic_command[2]==0x12)){
                ASIC_T2UserSelections();
            }            
        }
        count=0;
//      Empty incoming sequence array for next command                    
            memset(incoming_seq,0,sizeof(incoming_seq));

        ASIC_PowerDown(); //All voltages to ASIC goes down, just in case

    }; //end of Loop
}; //End of main
    /**
 End of File
*/