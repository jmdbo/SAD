#include <p24fxxxx.h>
#include <stdio.h>
#include <string.h>
#include "uartcom.h"


// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif

int initADC(){
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
	TRISDbits.TRISD6 = 1;
	TRISDbits.TRISD13 = 1;
	TRISAbits.TRISA0 = 0;
	TRISAbits.TRISA5 = 0;
	initADC();
	initUART();

	int i = 0;
	int ADCValue=0;

	while ( 1 ){
		if ( !PORTDbits.RD6){
			PORTAbits.RA5 = 1;
			for( i = 0 ; i < 20 ; i++){};
			PORTAbits.RA0 = 1;
			for( i = 0 ; i < 20000 ; i++){};
			PORTAbits.RA5 = 0;
			for( i = 0 ; i < 20 ; i++){};
			PORTAbits.RA0 = 0;
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
