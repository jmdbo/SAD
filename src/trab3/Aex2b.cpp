#include <stdio.h>
#include <math.h>

#define Data    0x378
#define Status  0x379
#define Control 0x37a


short _stdcall Inp32(short portaddr);
void _stdcall Out32(short portaddr, short datum);
 

// compile on Visual C++ Toolkit 2003 Command Prompt:

//				cl xxxx.cpp /link inpout32.lib 

short temp;
int i,j,m=0;
int delay= 30000;

void main(){

	for(;;){

		for (i=0;i< 1000; i++)   //sleep
			Out32 (Data, 0x01);

		
		
		for (i=0;i< 1000; i++)   //sleep
	 		Out32 (Data, 0x00);
	}
}