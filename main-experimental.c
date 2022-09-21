/*
 * File:   main9.c
 * Author: admin
 *
 * Created on 7 April, 2021, 12:34 PM
 */
// add delay header file 
// add frequency 
#define    FCY    29491200UL//10000000UL    // Instruction cycle frequency, Hz - required for __delayXXX() to work
#include<libpic30.h>

#include "xc.h"
#include<stdio.h>
#include<string.h>
#include <xc.h>


// PIC24FJ128GA106 Configuration Bit Settings

// 'C' source line config statements

// CONFIG3
#pragma config WPFP = WPFP511           // Write Protection Flash Page Segment Boundary (Highest Page (same as page 85))
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable bit (Segmented code protection disabled)
#pragma config WPCFG = WPCFGDIS         // Configuration Word Code Page Protection Select bit (Last page(at the top of program memory) and Flash configuration words are not protected)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select bit (Write Protect from WPFP to the last page of memory)

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT oscillator mode selected)
#pragma config IOL1WAY = ON             // IOLOCK One-Way Set Enable bit (Write RP Registers Once)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSCO functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-speed start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Standard Watchdog Timer is enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator functions are shared with PGEC1/PGED1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = ON              // JTAG Port Enable (JTAG port is enabled)


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

   char uart1_buffer[100];
    int uart_index;
// uart init funtion 

void UART1Init(void)
{
//	U1BRG = 25;		// 9600 @ 8MHZ
	U1BRG = 32;		// 9600 @ 10MHZ

	U1MODEbits.UARTEN = 1;		// UART2 is Enabled
	
	U1MODEbits.USIDL = 0;		// Continue operation at Idlestate
	U1MODEbits.IREN = 0;		// IrDA En/Decoder is disabled
	U1MODEbits.RTSMD = 0;		// flow control mode
	U1MODEbits.UEN1 = 0b00;		// UTX, RTX, are enabled U1CTS, U1RTS are disabled
	U1MODEbits.UEN0 = 0b00;		// UTX, RTX, are enabled U1CTS, U1RTS are disabled
	U1MODEbits.WAKE = 1;		// Wake-up on start bit is enabled
	U1MODEbits.LPBACK = 0;		// Loop-back is disabled
	U1MODEbits.ABAUD = 0;		// auto baud is disabled
	U1MODEbits.RXINV = 0;		// No RX inversion
	U1MODEbits.BRGH = 0;		// low boud rate
	U1MODEbits.PDSEL = 0b00; 	// 8bit no parity
	U1MODEbits.STSEL = 0;		// one stop bit	


	U1STAbits.UTXISEL1 = 0b00;		
	U1STA &= 0xDFFF;			// clear TXINV by bit masking
	U1STAbits.UTXBRK = 0;		// sync break tx is disabled
	U1STAbits.UTXEN = 1;		//transmit  is enabled
	U1STAbits.URXISEL = 0b00;	// interrupt flag bit is set when RXBUF is filled whith 1 character
	U1STAbits.ADDEN = 0;		// address detect mode is disabled
	U1STAbits.RIDLE = 0;

	IPC2bits.U1RXIP = 7;        // sET uart1 Priority to 7
	IFS0bits.U1RXIF = 0;		// clear interrupt flag of rx
	IFS0bits.U1TXIF = 0;		// clear interrupt flag of rx
	IEC0bits.U1RXIE = 1;		// enable rx recieved data interrupt
	IEC0bits.U1TXIE = 0;
}

// uart transmit function 


void UART1TransmitByte(char tx_byte)
{
	U1TXREG = tx_byte;
	while(!(U1STAbits.TRMT))
	{
	}
}


// uart read string function


void UART1TransmitString(char *tx_str)
{
	char ch;
	while(*tx_str != 0x00)
	{
		ch = *tx_str;
		U1TXREG = ch;
		while(!(U1STAbits.TRMT))
		{
		}
		tx_str++;
	}
}


// write code for receiving data using interrupt 


void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt()
{
	char ch;
 
	_U1RXIF = 0;
	ch = U1RXREG;
    
    uart1_buffer[uart_index++]=ch;
    
    if(uart_index>300)uart_index=0;
    
	
}

int main(void) {
    
    
    // uart pin selection 
    OSCCON = 0x0000;
    OSCTUN = 0x0000;
    CLKDIV = 0x0000;
    //resetSource = RCON;
    RCON = 0x00;
    INTCON1bits.NSTDIS = 1;
    AD1PCFGL = 0xFF; //dISABLE ANALOG I/P'S
    
    RPINR18bits.U1RXR = 6; // Assign U1RXR to RP6
    _TRISB6 = 1; //CONFIGURE PIN TO I/P
    RPOR3bits.RP7R = 3; //Assign U1TX To Pin RP7
    _TRISB7 = 0; //CONFIGURE PIN TO O/P
    
     
      
    //uart_index=0;
    CNPU1bits.CN9PUE = 1;
    
    UART1Init();
        UART1TransmitString("MEVIHUB.COM\r\n");
        __delay_ms(500);
    while(1)
    {
    
       if(uart_index>0)
       {
           UART1TransmitString(uart1_buffer);
           uart_index = 0;
           memset(uart1_buffer,'\0',sizeof(uart1_buffer));
       }
    }
    
    /// transmission completed 
    
    
    return 0;
}