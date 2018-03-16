
#include "delay.h"

static uint8_t  s_chFacMicro = 0;
static uint16_t s_hwFacMill = 0;


void delay_init(uint32_t wSystemCoreClock)	 
{

}								    


void delay_us(uint32_t wMicro)
{	
  int a,b=0;
  for (int a = 0;a<300;a++)
  {
    b=a*a*a;
  }     
}


void delay_ms(uint16_t hwMill)
{	 		  	  
	uint32_t a;
	
        for(a=0; a<hwMill; a++){
          delay_us(1);
        }
            
} 


