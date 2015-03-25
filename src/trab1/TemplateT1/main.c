#include <p24fxxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "uartcom.h"
#include "lcd.h"


// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif


int initADC(){ //potenciometro
	//AD1PCFG = 0xFFDF; 		// AN5 as analog, all other pins are digital
	AD1PCFG = 0xFFFB;		//AN2
	AD1CON1 = 0x0000; 		// SAMP bit = 0 ends sampling
							// and starts converting
	AD1CHS = 0x0005; 		// Connect AN5 as CH0 input
							// in this example AN2 is the input
	AD1CSSL = 0;
	AD1CON3 = 0x0002; // Manual Sample, Tad = 2 Tcy
	AD1CON2 = 0;
	AD1CON1bits.ADON = 1; // turn ADC ON
	

	return 1;
}

int main(void)
{
	TRISDbits.TRISD6 = 1; //botao
	TRISDbits.TRISD13 = 1;
	TRISAbits.TRISA0 = 0;
	TRISAbits.TRISA5 = 0;
	initADC();
	initUART();
	LCDInit();

	long int i = 0;
	int PotValue=0;
	int TempValue=0;
	char buff[50];
	int pw_entered=0;
	int state=0;
	long int delay=20000;
	
	//Update PotValue
	


	while ( 1 ){
		
		/***********Update PotValue ********************/
		AD1PCFG = 0xFFDF;
		AD1CHS = 0x0005; 
		AD1CON1bits.SAMP = 1; 		// start sampling...
		for( i = 0 ; i < 200 ; i++){};
		 		// Ensure the correct sampling time has elapsed
									// before starting conversion.
		AD1CON1bits.SAMP = 0; 		// start Converting
		while (!AD1CON1bits.DONE); 	// conversion done?
		PotValue = ADC1BUF0;
		sprintf(buff,"P: %4d",PotValue);
		//LCDClear();
		LCDL1Home();
		putstringLCD(buff);

		/***********Update PotValue ********************/
	
		/**************Update Temperature **************/

		AD1PCFG = 0xFFFB;
		AD1CHS = 0x0002;
		AD1CON1bits.SAMP = 1; 		// start sampling...
		for( i = 0 ; i < 200 ; i++){};
		 		// Ensure the correct sampling time has elapsed
									// before starting conversion.
		AD1CON1bits.SAMP = 0; 		// start Converting
		while (!AD1CON1bits.DONE); 	// conversion done?
		TempValue = ADC1BUF0;
		sprintf(buff," T: %4d",TempValue);
		putstringLCD(buff);
		
		/**************Update Temperature **************/
		
		/************** SET DELAY **********************/
		if(PotValue>=0 && PotValue<=200){
			delay = 100000;
		}
		if(PotValue>200 && PotValue <=400){
			delay = 80000;
		}
		if(PotValue>400 && PotValue <=600){
			delay = 60000;
		}
		if(PotValue>800 && PotValue <=1000){
			delay = 40000;
		}
		if(PotValue>1000){
			delay = 20000;
		}
		/************** SET DELAY **********************/
		
		
		/*********** Check if there is something in the UART ***************************/
		if(U2STAbits.URXDA){
			char rec = getcharUART();
			if(rec=='T' || rec=='t'){
				putcharUART(rec);
				putstringUART("\n");
				sprintf(buff,"Temperature: %d\n", TempValue);
				putstringUART(buff);
			}	
			if(rec=='P' || rec=='p'){
				putcharUART(rec);
				putstringUART("\n");
				sprintf(buff,"Potentiometer: %d\n", PotValue);
				putstringUART(buff);
			}
		}
		/*********** Check if there is something in the UART ***************************/	
		
		/***************** System RESET by buttons ************************************/
		if(!PORTDbits.RD7 && !PORTDbits.RD13 && state==1){
			pw_entered=0;
			putstringUART("Palavra Passe\r\n");
			while(!pw_entered){
				if(U2STAbits.URXDA){
					getstringUART(buff,47);
					if(!strcmp(buff,"boi")){
						putstringUART("\r\n");
						putstringUART("Pois es! Password aceite!\r\n");
						LCDClear();
						state=0;
					}else{
						putstringUART("\r\n");
						putstringUART(buff);
						putstringUART("Oh boi, aprende a pass!\r\n");
					}
					pw_entered=1;
				}
			}
		}
		/***************** System RESET by buttons ************************************/
		if ( state==1){

			PORTAbits.RA5 = 1; //led
			for( i = 0 ; i < 20 ; i++){};
			PORTAbits.RA0 = 1; //liga led
			for( i = 0 ; i < delay ; i++){};
			PORTAbits.RA5 = 0; //led
			for( i = 0 ; i < 20 ; i++){};
			PORTAbits.RA0 = 0; //desliga led
			for( i = 0 ; i < delay ; i++){};
			
		}
		//writeUART("Isto eh um teste\0");
		if ((TempValue>180 && state==0) || !PORTDbits.RD13){
			putstringUART("ALERTA! INCENDIO!\r\n");
			LCDL2Home();
			putstringLCD("ALARME ACTIVO!!!");
			state=1;			
		}
	}
}
