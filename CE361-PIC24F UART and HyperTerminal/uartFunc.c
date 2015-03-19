/**********************************************************************
* 2011 Microchip Technology Inc.
*
* FileName:        uartFunc.c
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
* Mark Pallones       04/01/11        Placed clrscr() and home() after initUART()
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code Tested on:
* Explorer 16 Demo board with  PIC24FJ128GA010 controller and Windows Hyperterminal program
* see READ ME.txt included on this folder for more information on how to test the code 
* DESCRIPTION:
* This code example describes how to use UART on PIC24F. Here I can send a specific question
* and PIC24 will reply a specific answer. The user can type the questions in Hyperterminal and send to PIC24 via RS232   
*************************************************************************************************************************/

#include <p24fj128ga010.h>
#include "uartcom.h"

// initialize the UART2 serial port
void initUART( void)
{
	U2BRG 	= BRATE;    
	U2MODE 	= U_ENABLE;
	U2STA 	= U_TX;
	TRTS    = 0;        // make RTS output
	RTS     = 1;        // set RTS default status
} // initUART


// send a character to the UART2 serial port
int putcharUART( int c)
{
	while ( CTS);		        // wait for !CTS, clear to send
	while ( U2STAbits.UTXBF);   // wait while Tx buffer full
	U2TXREG = c;
	return c;
} // putcharUART

void putstringUART( char *s)
{
   
	while( *s)			// loop until *s == '\0' the  end of the string
    	
  	putcharUART( *s++);	// send the character and point to the next one
    putcharUART( '\r');       // terminate with a cr / line feed
} // putstringUART


// wait for a new character to arrive to the UART2 serial port
char getcharUART( void)
{
    RTS = 0;            // assert Request To Send !RTS
	while ( !U2STAbits.URXDA);	// wait for a new character to arrive
	RTS = 1;
	return U2RXREG;		// read the character from the receive buffer
}// getcharUART

char *getstringUART( char *s, int len)
{
    char *p = s;            // copy the buffer pointer 
    do{
        *s = getcharUART();       // wait for a new character
        putcharUART( *s);         // echo character
        
        if (( *s==BACKSPACE)&&( s>p))
        {
            putcharUART( ' ');     // overwrite the last character
            putcharUART( BACKSPACE);
            len++;
            s--;            // back the pointer
            continue;
        }
        if ( *s=='\n')      // line feed, ignore it
            continue;
        if ( *s=='\r')      // end of line, end loop
            break;          
        s++;                // increment buffer pointer
        len--;
    } while ( len>1 );      // until buffer full
 
    *s = '\0';              // null terminate the string 
    
    return p;               // return buffer pointer
} // getstringUART



