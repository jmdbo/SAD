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

		temp = Inp32 (Status);
		printf(" Dados: %d", temp );

		if(temp==126){
				for (i=0;i< 1000; i++)   //sleep
				Out32 (Data, 0x01);
				for (i=0;i< 1000; i++)   //sleep
				Out32 (Data, 0x00);
				for (j=0;j<9; j++){ 
					  
					if(temp==64)
					//printf("\n%d\n", m );
					for (i=0;i< delay; i++)   //sleep
						Out32 (Data, m);
					for (i=0;i< delay; i++)   //sleep
						Out32 (Data, 0);
					if(j==0) m=1;
					else m=2*m;
				}
				m=0;
				

		}
		if(temp==222){
				for (i=0;i< 3000; i++)   //sleep
				Out32 (Data, 0x01);
				for (i=0;i< 3000; i++)   //sleep
				Out32 (Data, 0x00);
				for (j=0;j<9; j++){ 
					  
					if(temp==64)
					//printf("\n%d\n", m );
					for (i=0;i< delay; i++)   //sleep
						Out32 (Data, m);
					for (i=0;i< delay; i++)   //sleep
						Out32 (Data, 0);
					if(j==0) m=1;
					else m=2*m;
				}
				m=0;
			

		}
	if(temp==254){		for (i=0;i< 1000; i++)   //sleep
					Out32 (Data, 0x01);
					for (i=0;i< 1000; i++)   //sleep
					Out32 (Data, 0x00);
					m=128;
					for (j=0;j<9; j++){ 
					  
					if(j==8) m=0;
					else if(m!=0) m=m/2;
					//printf("\n%d\n", m );
					for (i=0;i< delay; i++)   //sleep
						Out32 (Data, m);
					for (i=0;i< delay; i++)   //sleep
						Out32 (Data, 0);
					
				}
				m=0;
				

		}
	if(temp==94){		
					for (i=0;i< 3000; i++)   //sleep
					Out32 (Data, 0x01);
					for (i=0;i< 3000; i++)   //sleep
					Out32 (Data, 0x00);
					m=128;
					for (j=0;j<9; j++){ 
					  
					if(j==8) m=0;
					else if(m!=0) m=m/2;
					//printf("\n%d\n", m );
					for (i=0;i< delay; i++)   //sleep
						Out32 (Data, m);
					for (i=0;i< delay; i++)   //sleep
						Out32 (Data, 0);
					
				}
				m=0;
				

		}

		
	}

}


/* exe.2
		for (i=0;i< 1000; i++)   //sleep
			Out32 (Data, 0x01);

		//Out32 (Data, temp);
		
		for (i=0;i< 1000; i++)   //sleep
			Out32 (Data, 0x00);


		//printf("Dados: %d\n", temp);
*/