#include <p24fxxxx.h>
#include <stdio.h>
#include <string.h>

// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif


int main(void)
{
	TRISDbits.TRISD6 = 1;
	TRISAbits.TRISA0 = 0;

	int i = 0;

	while ( 1 ){
		if ( !PORTDbits.RD6 ){
			PORTAbits.RA0 = 1;
			for( i = 0 ; i < 20000 ; i++){};
			PORTAbits.RA0 = 0;
			for( i = 0 ; i < 20000 ; i++){};
		}
	}
}
