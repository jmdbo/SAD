/*****************************************************************************
 *
 * LCD Driver for PIC24.
 *
 *****************************************************************************
 * FileName:        lcd.c
 * Dependencies:    
 * Processor:       
 * Compiler:       	
 * Linker:          
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") is intended and supplied to you, the Company's
 * customer, for use solely and exclusively with products manufactured
 * by the Company. 
 *
 * The software is owned by the Company and/or its supplier, and is 
 * protected under applicable copyright laws. All rights are reserved. 
 * Any use in violation of the foregoing restrictions may subject the 
 * user to criminal sanctions under applicable laws, as well as to 
 * civil liability for the breach of the terms and conditions of this 
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, 
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * A simple LCD driver for LCDs interface through the PMP
 * 
 *
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Vidyadhar           30/June/06	...	
 * Brant Ivey			7/3/07		Updated for v4.0 of class
 *****************************************************************************/

#include	<p24FJ128GA010.h>
#include "lcd.h"

// Looptime is 1 us per loop count

// Define a fast instruction execution time in terms of loop time
// typically > 43us
#define	LCD_F_INSTR		50

// Define a slow instruction execution time in terms of loop time
// typically > 1.53ms
#define	LCD_S_INSTR		1600

// Define the startup time for the LCD in terms of loop time
// typically > 40ms
#define	LCD_STARTUP		50000


unsigned int	_uLCDloops;



void LCDInit()
{
	PMMODE = 0x3FF;
//PMMODEbits.BUSY = 0;
	PMCON = 0x8303;
	PMAEN = 0x0001;

	_uLCDloops = LCD_STARTUP;
	Wait(_uLCDloops);

	_uLCDloops = LCD_F_INSTR;
	PMDIN1 = 0b00111000;			// Set the default function
	Wait(_uLCDloops);

	_uLCDloops = LCD_STARTUP;
	Wait(_uLCDloops);

	_uLCDloops = LCD_F_INSTR;
	PMDIN1 = 0b00001100;
	Wait(_uLCDloops);

	_uLCDloops = LCD_STARTUP;
	Wait(_uLCDloops);

	_uLCDloops = LCD_S_INSTR;
	PMDIN1 = 0b00000001;			// Clear the display
	Wait(_uLCDloops);

	_uLCDloops = LCD_STARTUP;
	Wait(_uLCDloops);

	_uLCDloops = LCD_S_INSTR;
	PMDIN1 = 0b00000110;			// Set the entry mode
	Wait(_uLCDloops);

	LCDClear();
	LCDHome();
}



void LCDHome()
{
	_uLCDloops = LCD_S_INSTR;
	PMADDR = 0x0000;
	PMDIN1 = 0b00000010;
	while(_uLCDloops)
	_uLCDloops--;
}


void LCDL1Home()
{
	_uLCDloops = LCD_S_INSTR;
	PMADDR = 0x0000;
	PMDIN1 = 0b10000000;
	while(_uLCDloops)
	_uLCDloops--;
}

void LCDL2Home()
{
	_uLCDloops = LCD_S_INSTR;
	PMADDR = 0x0000;
	PMDIN1 = 0b11000000;
	while(_uLCDloops)
 	_uLCDloops--;
}


void LCDClear()
{
	_uLCDloops = LCD_S_INSTR;
	PMADDR = 0x0000;
	PMDIN1 = 0b00000001;
	while(_uLCDloops)
	_uLCDloops--;
}

void LCDPut(char A)
{
	_uLCDloops = LCD_F_INSTR;
	PMADDR = 0x0001;
	PMDIN1 = A;
	while(_uLCDloops)
	_uLCDloops--;
	Nop();
	Nop();
	Nop();
	Nop();
}



void Wait(unsigned int B)
{
	while(B)
	B--;
}

void putstringLCD(char *s){
	while( *s)			// loop until *s == '\0' the  end of the string
	LCDPut(*s++);	
}	


