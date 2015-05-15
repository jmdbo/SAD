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
int sensor_garage=0;
int sensor_alarm=0;
int sensor_street=0;
int door=0;
int fire=0;


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
		I2C_State=I2C2RCV;  
		if (I2C2STATbits.R_W == 1)
		{
			// write data
			I2C2TRN=action;//I2CDataToSend;

		}           
	} else {
		if (I2C2STATbits.R_W == 1)
		{
        	// write data.
        	//n�o fazer nada
		} else 
		{
			// read data
			I2C_State=I2C2RCV;
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
	MRF24J40_setChannel(CHANNEL_16);    //definir o canal para comunica��o

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
	// led 5 mandar avan�ar
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
	z u*/
}

// comand RF builder 
//1 if sucess
//0 if no sucess
//buff return the comand value
// comand 0-3
int comand(unsigned char* buff,int comand){
	int i;
	if(comand<0 || comand >3)
		return 0;
	for(i=0;i<4;i++)
	{
		if(i==comand)
			buff[i]=1;
		else
			buff[i]=0;
	}
	
	return 1;	
}	

int send_comandI2C(int comand_to_send)
{
	if(i>=0 && i<=5){
		action = comand_to_send;
		return 1;
	}
	return 0;
}

//Received comand form i2c.
int receive_comandI2C(){
	if(I2C_State==1)
	{
		sensor_alarm=0;
		sensor_garage=0;
		sensor_street=0;
	}
	if(I2C_State==10){
		sensor_garage=1;
	}
	if(I2C_State==20){
		sensor_street=1;
	}
	if(I2C_State==30){
		door=1;
	}
	if(I2C_State==40){
		door=0;
	}
	if(I2C_State==50){
		sensor_alarm=1;
	}
	if(I2C_State==60){
		sensor_alarm=1;
	}
	if(I2C_State==70){
		fire=1;
	}

	return 1;	
}

int main(void)
{    
	estado = 0;
	action = 5;
	int comand_sucess=0;
	inicializar();

    while(1)
	{
		if(RF_INT_PIN==0)
		{
			RFIF=1;	
		}

		if (estado == 1)
		{
			//sensor state
			comand(bufOUT,3);
			buf = sendRFMessages(bufOUT,0,0);
			if(buf.byte[3]){
				estado = 2;
			}	
			
		}
		if (estado == 0)
		{
				bufOUT[0]=0;
				bufOUT[1]=1;
				bufOUT[2]=0;
				bufOUT[3]=0;
				buf =sendRFMessages(bufOUT,0,0);
				if(buf.byte[0]){
					estado = 1;
					action= 20;
				}		
				//MRF24J40_send(bufOUT, sizeof(bufOUT));
			
			
			// Estados intermedios
			if(sensor_garage==0 && street_sensor==0 && sensor_alarm==0)
			{
				// move foward
				comand(buffOUT,1);
			}
			if(sensor_street==1 && door=0 && sensor_alarm==0){
				//open door
				comand(buffOUT,0);
				send_comandI2C(1);
			}
			if(sensor_street==1 && door=1 && sensor_alarm==0){
				// move foward inside garage
				command(buffOUT,1);
			}
			if(sensor_garage==1 && door=1 && sensor_alarm==0){
				//Stop car and close door
				comand(buffOUT,0);
				send_comandI2C(2);
				garage=1;
			}
			if(sensor_alarm==1 && garage==1 && door ==1){
				//open door
				send_comandI2C(1);
			}
			if(sensor_garage==1 && garage==1 && door==0){
				//move backward
			}
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