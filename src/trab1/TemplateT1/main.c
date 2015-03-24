#include <p24fxxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "uartcom.h"


// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif


int initADC(){ //potenciometro
	AD1PCFG = 0xFFDF; 		// AN5 as analog, all other pins are digital
	//AD1PCFG = 0xFFFB;		//AN2
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

	long int i = 0;
	int ADCValue=0;
	char buff[50];
	int pw_entered=0;


	while ( 1 ){
		if(U2STAbits.URXDA){
			char rec = getcharUART();
			if(rec=='T')
				putstringUART("Received String\n");
			if(rec=='P'){
				sprintf(buff,"Potentiometer: %d\n", ADCValue);
				putstringUART(buff);
			}
		}	
		if(!PORTDbits.RD7 && !PORTDbits.RD13){
			pw_entered=0;
			putstringUART("Palavra Passe\r\n");
			while(!pw_entered){
				if(U2STAbits.URXDA){
					getstringUART(buff,47);
				}
			}
		}
		if ( !PORTDbits.RD6){
			/*
			
		PORTAbits.RA0 = 1; //liga led
		if(ADCValue>=0 || ADCValue<=200){
			for( i = 0 ; i < 100000 ; i++){};
		}
		if(ADCValue>200 || ADCValue <=400){
			for( i = 0 ; i < 80000 ; i++){};
		}
		if(ADCValue>400 || ADCValue <=600){
			for( i = 0 ; i < 60000 ; i++){};
		}
		if(ADCValue>800 || ADCValue <=1000){
			for( i = 0 ; i < 40000 ; i++){};
		}
		if(ADCValue>1023){
			for( i = 0 ; i < 20000 ; i++){};
		}
		PORTAbits.RA0 = 0; // desliga led
			
			*/
			PORTAbits.RA5 = 1; //led
			for( i = 0 ; i < 20 ; i++){};
			PORTAbits.RA0 = 1; //liga led
			for( i = 0 ; i < 20000 ; i++){};
			PORTAbits.RA5 = 0; //led
			for( i = 0 ; i < 20 ; i++){};
			PORTAbits.RA0 = 0; //desliga led
			for( i = 0 ; i < 20000 ; i++){};
			putstringUART("Blinking!!!\n");
		}
		//writeUART("Isto eh um teste\0");
		if (!PORTDbits.RD13){
			
			AD1CON1bits.SAMP = 1; 		// start sampling...
			for( i = 0 ; i < 200 ; i++){};
			 		// Ensure the correct sampling time has elapsed
										// before starting conversion.
			AD1CON1bits.SAMP = 0; 		// start Converting
			while (!AD1CON1bits.DONE); 	// conversion done?
			ADCValue = ADC1BUF0;
			if(ADCValue>=1){
				//writeUART("Isto eh um teste\0");
				PORTAbits.RA5 = 1;
			}else PORTAbits.RA5=0;
		}
	}
}
