/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below 
* Processor:       PIC24FJ128GA010
* Compiler:        MPLAB C30 v3.11 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all 
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your 
* behalf, exclusively with Microchip's proprietary products.  Your 
* acceptance and/or use of this code constitutes agreement to the 
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT 
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS 
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH 
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE 
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, 
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR 
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER 
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP 
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Mark Pallones		02/25/11		First Release
* Mark Pallones     04/01/11        Placed clrscr() and home() after initUART()
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
* Code Tested on:
* Explorer 16 Demo board with  PIC24FJ128GA010 controller and Windows Hyperterminal program
* see READ ME.txt included on this folder for more information on how to test the code 
* DESCRIPTION:
* This code example describes how to use UART on PIC24F. Here I can send a specific question
* and PIC24 will reply a specific answer. The user can type the questions in Hyperterminal and send to PIC24 via RS232   
*************************************************************************************************************************/
           
// init the serial port (UART2, 9600@32MHz, 8, N, 1, CTS/RTS )
void initUART( void);

// send a character to the serial port
int putcharUART( int c);

// wait for a new character to arrive to the serial port
char getcharUART( void);

// send a null terminated string to the serial port
void putstringUART( char *s);

// receive a null terminated string in a buffer of len char
char * getstringUART( char *s, int n);


// I/O definitions for the Explorer16
#define CTS 	_RF12		        // Cleart To Send, input, HW handshake
#define RTS     _RF13               // Request To Send, output, HW handshake
#define TRTS    TRISFbits.TRISF13   // tris control for RTS pin

// timing and baud rate calculations
#define BRATE    12         // 19200 baud (BREGH=1)
#define U_ENABLE 0x8000		// enable the UART peripheral (BREGH=1)
#define U_TX	 0x0400		// enable transmission
#define BACKSPACE 0x8   // ASCII backspace character code 

// useful macros
#define clrscr() putstringUART( "\x1b[2J") 
#define home()   putstringUART( "\x1b[H") 
#define pcr()    putcharUART( '\r')
