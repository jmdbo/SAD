#include <p24fxxxx.h>
#include <stdio.h>
#include <string.h>

#define BAUDRATE       9600
#define BRGVAL          ((FCY/BAUDRATE)/16)-1

// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif

int initADC(){
	AD1PCFG = 0xFFDF; 		// AN2 as analog, all other pins are digital
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

int initUART(){

	U1BRG=9600; 	  	  //Set Baudrate
	IPC3bits.U1TXIP2 = 1; //Set Uart TX Interrupt Priority
	IPC3bits.U1TXIP1 = 0;
	IPC3bits.U1TXIP0 = 0;
	IPC2bits.U1RXIP2 = 1; //Set Uart RX Interrupt Priority
	IPC2bits.U1RXIP1 = 0;
	IPC2bits.U1RXIP0 = 0;
	
	U1STA = 0;
	U1MODE = 0x8000; //Enable Uart for 8-bit data
	
	//no parity, 1 STOP bit
	U1STAbits.UTXEN = 1; //Enable Transmit
	IEC0bits.U1TXIE = 1; //Enable Transmit Interrupt
	IEC0bits.U1RXIE = 1; //Enable Receive Interrupt

	return 1;
}

 void writeUART(char *str2Write)
 {
 	while (*str2Write != '\0')
 	{
 		while (!U1STAbits.TRMT);
 		U1TXREG = *str2Write;
 		*str2Write++;
 	}
 }

int main(void)
{
	TRISDbits.TRISD6 = 1;
	TRISDbits.TRISD13 = 1;
	TRISAbits.TRISA0 = 0;
	TRISAbits.TRISA5 = 0;
	initADC();
	//initUART();

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
		
		}
		if (!PORTDbits.RD13){
			
			AD1CON1bits.SAMP = 1; 		// start sampling...
			for( i = 0 ; i < 200 ; i++){};
			 		// Ensure the correct sampling time has elapsed
										// before starting conversion.
			AD1CON1bits.SAMP = 0; 		// start Converting
			while (!AD1CON1bits.DONE); 	// conversion done?
			ADCValue = ADC1BUF0;
			if(ADCValue>=1000){
				PORTAbits.RA5 = 1;
			}else PORTAbits.RA5=0;
		}
	}
}
