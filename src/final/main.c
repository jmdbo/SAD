#include <p24fxxxx.h>
#include "MRF24J40.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Setup configuration bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif

int estado;
BUFFER buf;
unsigned char bufOUT[4];

int okIn;
int sensorCarro;
int alarmeCarro;

int action;

int I2C_State;

int garage=0;

/*
Functions to use:
  void MRF24J40_setChannel(BYTE channel)     - Sets the current operating channel 
  BOOL MRF24J40_newMsg( void )               - Checks if a new message have arrived
  BYTE* MRF24J40_get( void )                 - Gets the received message
  void MRF24J40_send(BYTE* txPck, BYTE len)  - Sends a message
  void MRF24J40_initialize()                 - Inicialize
*/

void __attribute__((interrupt, auto_psv)) _SI2C2Interrupt(void)
{
	IFS3bits.SI2C2IF = 0;     //disable flag
    IEC3bits.SI2C2IE = 0;
    I2C2CONbits.SCLREL = 0;
       
	if(I2C2STATbits.D_A == 0) 
	{	
		// read address
		garage=I2C2RCV;  
		if (I2C2STATbits.R_W == 1)
		{
			// write data
			I2C2TRN=action;//I2CDataToSend;

		}           
	} else {
		if (I2C2STATbits.R_W == 1)
		{
        	// write data.
        	garage=I2C2RCV;
		} else 
		{
			// read data
			garage=I2C2RCV;
			//update_sensors(estado);
        }
	}

    I2C2CONbits.SCLREL = 1;
   	IEC3bits.SI2C2IE = 1;
}

void inicializar()
{
  // Botoes ****************************************************
	// 1-envio, 0-receber
	TRISDbits.TRISD6 = 1;   //B1

	TRISAbits.TRISA0 = 0;   //LED
	TRISAbits.TRISA1 = 0;   //LED
	TRISAbits.TRISA2 = 0;   //LED
	TRISAbits.TRISA3 = 0;   //LED
	TRISAbits.TRISA4 = 0;   //LED
	TRISAbits.TRISA5 = 0;   //LED
	TRISAbits.TRISA6 = 0;   //LED
	TRISAbits.TRISA7 = 0;   //LED

	// config RF
	MRF24J40_initialize();    	    //inicializar sistema de radio frequencia ?????
	MRF24J40_setChannel(CHANNEL_16);    //definir o canal para comunicação

	// config I2C 
	I2C2CON = 0b1001100011000000;  
	I2C2ADD = 2;
	IEC3bits.SI2C2IE = 1;
}

BUFFER sendRFMessages(unsigned char bufOUTMesages[4], int sensorPortaDecide, int alarmDecide)
{
	/*
	int bST = bufOUTMesages[0];
	int bFR = bufOUTMesages[1];
	int bBA = bufOUTMesages[2];
	int bSS = bufOUTMesages[3];
	*/
	int bOk;
	int bSensorPorta;
	int bAlarm;
	
	int i;

	static BUFFER bufMessages;

	// led 0 alarme
	// led 4 mandar parar
	// led 5 mandar avançar
	// led 6 mandar recuar
	/* led 7 pedir sensor
	Led4 = 0;
	for (i = 0;i < 10; i++);
	Led5 = 0;
	for (i = 0;i < 10; i++);
	Led6 = 0;
	for (i = 0;i < 10; i++);
	Led7 = 0;*/

	bOk = 0; bSensorPorta = 0; bAlarm = 0;
	MRF24J40_send(bufOUTMesages, 4);
	while(!MRF24J40_newMsg()){
		for(i=0; i<200; i++);
	}
	bufMessages = MRF24J40_get();
	return bufMessages;
	/*
	if (bST == 1)
	{
		bOk = 1;
		Led4 = 1;
		for (i = 0;i < 10; i++);
		Led5 = 0;
		for (i = 0;i < 10; i++);
		Led6 = 0;
		for (i = 0;i < 10; i++);
		Led7 = 0;
	} else
	{
		if (bFR == 1)
		{
			bOk = 1;
			Led4 = 0;
			for (i = 0;i < 10; i++);
			Led5 = 1;
			for (i = 0;i < 10; i++);
			Led6 = 0;
			for (i = 0;i < 10; i++);
			Led7 = 0;
		} else
		{
			if (bBA == 1)
			{
			bOk = 1;
			Led4 = 0;
			for (i = 0;i < 10; i++);
			Led5 = 0;
			for (i = 0;i < 10; i++);
			Led6 = 1;
			for (i = 0;i < 10; i++);
			Led7 = 0;
			}	
		}
	}
	if (bSS == 1)
	{
		if (sensorPortaDecide == 0)
		{
			bSensorPorta = 0;
		} else
		{
			bSensorPorta = 1;
		}
		Led4 = 0;
		for (i = 0;i < 10; i++);
		Led5 = 0;
		for (i = 0;i < 10; i++);
		Led6 = 0;
		for (i = 0;i < 10; i++);
		Led7 = 1;
	}
	if (alarmDecide == 0)
	{
		bAlarm = 0;
		Led0 = 0;
	}
	if (alarmDecide == 1)
	{
		bAlarm = 1;
		Led0 = 1;
	}

	bufMessages.byte[0] = bOk;
	bufMessages.byte[1] = bSensorPorta;
	bufMessages.byte[2] = bAlarm;
	return 	bufMessages;
	*/
}

int main(void)
{    
	estado = 0;
	action = 5;
	int portaAberta=0;
	int pedidoLuz=0, luzAlarme=0;
	//int carroEmMovimento=0;
	int alarme=0;
	BUFFER receivedBuf;
	BOOL newMsg;
	//int comand_sucess=0;
	inicializar();

    while(1)
	{
		
		if(RF_INT_PIN==0)
		{
			RFIF=1;	
		}
		newMsg=MRF24J40_newMsg();
		if(newMsg){
			receivedBuf= MRF24J40_get();
			alarme=receivedBuf.byte[2];				
		}	
		if(garage == 70 && luzAlarme==0 && pedidoLuz ==0){
			action = 0x03;
			pedidoLuz=1;
			
		}
		if(pedidoLuz==1 && garage== 50){
			//Luz Alarme Ligada
			pedidoLuz = 0;
			luzAlarme = 1;
			if(estado==4){
				estado = 5;
			}
			action=5;	
			
		}
		if(pedidoLuz == 1 && garage==60){
			pedidoLuz = 0;
			luzAlarme = 0;
			action = 5;
			if(estado==8){
				estado=0;
			}	
		}	
		if(estado == 8 && garage == 40){
			portaAberta=0;
			action =5;
		}	
		if(estado == 7 && garage == 1){
			estado =8;
			action = 2;
			
		}	
		if(estado == 6 && alarme){
			bufOUT[0]=0;
			bufOUT[1]=0;
			bufOUT[2]=1;
			bufOUT[3]=0;
			MRF24J40_send(bufOUT, sizeof(bufOUT));
			estado = 7;
			action = 5;
			
		}
		if(luzAlarme == 1 && alarme ==0 && pedidoLuz==0){
			pedidoLuz=1;
			action=0x04;
		}				
		if(estado == 5 ){
			if(portaAberta){
				estado = 6;					
			}else{
				action = 0x01;
			}
			if(garage==30){
				estado =6;
				portaAberta=1;
			}		
		}
			
		if(estado == 4 && garage == 40){
			portaAberta=0;
			action = 5;
			
		}	
		
		if(estado == 3 && garage== 10){
			action = 0x02;
			estado = 4;
			PORTAbits.RA0 = 1;
		}	
		
		if(estado ==2 && garage==30){
			//move car forward
			bufOUT[0]=0;
			bufOUT[1]=1;
			bufOUT[2]=0;
			bufOUT[3]=0;
			MRF24J40_send(bufOUT,sizeof(bufOUT));
			estado=3;
			portaAberta=1;
			action = 5;
			
		}	

		if (estado == 1 && garage==20)
		{
			//sensor state
			action = 0x01;
			estado = 2;	
		}
		if (estado == 0)
		{
				bufOUT[0]=0;
				bufOUT[1]=1;
				bufOUT[2]=0;
				bufOUT[3]=0;
				estado = 1;
				MRF24J40_send(bufOUT,sizeof(bufOUT));
				//if(buf.byte[0]){
					
				//}	
				//MRF24J40_send(bufOUT, sizeof(bufOUT));
			
		
		}
		
		/*

		int okIn = buf.byte[0];
		int sensorPorta = buf.byte[1];
		int alarm = buf.byte[2];
		
		if (MRF24J40_newMsg())
		{	
			buf = MRF24J40_get();
			okIn= buf[0]; // Actouo 1 ou nao 0
			sensorCarro=buf[1]; //Detecta 1 nao 0
			alarmeCarro=buf[2]; // Alarme activo 1 sem alarme 0
		}
		*/
	}
}
