/**********************************************************************
* 2011 Microchip Technology Inc.
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
*+
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Mark Pallones	   02/25/11		First Release
* Mark Pallones    04/01/11        Placed clrscr() and home() after initUART()
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
#include <stdio.h>


_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & ICS_PGx2 & FWDTEN_OFF)
_CONFIG2(IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF &  FNOSC_PRIPLL & POSCMOD_HS)

main()
{
   
    char s[128];
    char iam[]=("\n>>I am PIC24FJ128GA010 mounted in Explorer16 Demo Board \n");
    char sor[]=("\n>>Sorry I don't understand you\n");
    char cre[]=("\n>>The Microchip \n");
    char wro[]=("\n>>MARK PALLONES \n");
    char ama[]=("\n>>AMAD Application Engineer\n");
   
    initUART();  // 1. init the UART2 serial port 
	
    clrscr();
    home();
   
  
	while (1)
	{   
        int i;
        for (i=0;i<0xFFFF;i++); // add delay
		
        getstringUART( s, sizeof(s));
		
        if (strcmp(s,"who are you?")==0)	 
        { 
        
     	putstringUART(iam);
        }
        	else if (strcmp(s,"clear screen")==0)
        	{ 
          		clrscr();
        	}
        		 else if (strcmp(s,"home")==0)
        		{
           		home();
        		}
                 
                 else if (strcmp(s,"who wrote your program?")==0)
                    {
                     putstringUART(wro);
                    }
                    else if (strcmp(s,"who created you?")==0)
                    {
                     putstringUART(cre); 
                    }
                      else if(strcmp(s,"who is Mark Pallones?")==0)
                      {
                       putstringUART(ama);  
                       }                  
        			else
        			{
        			putstringUART(sor);  
        			}
      	
	} // main loop
	    pcr(); 
}// main
