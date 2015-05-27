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
int i,j;
int delay= 30000;

void main(){

	for(;;){

		temp = Inp32 (Status);
		printf(" Dados: %d", temp );

		if(temp==126){
				
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x01);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
	
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x02);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
				
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x04);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
	
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x08);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
				
					
				
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x10);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
				
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x20);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x40);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x80);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);

		}
		if(temp==254){
								
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x80);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x40);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x20);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
	
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x10);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
				
					
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x08);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
				
					
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x04);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x02);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);
				
				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x01);

				for (i=0;i< delay; i++)   //sleep
					Out32 (Data, 0x00);


}
	
		
	}

}
