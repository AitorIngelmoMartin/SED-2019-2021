#include "lpc17xx.h"

#define DAC_BIAS	0x00010000  // Settling time a valor 2,5us

void DACInit(void);


int main (void)
{
  uint32_t i = 0, m;
  
  // Inicializamos el DAC DAC  
  DACInit();
  
  while (1)
  {
		LPC_DAC->DACR = (i << 6) | DAC_BIAS;	
		i++;
	
		for(m = 1000; m > 1; m--);
		if ( i == 1024 )
			i = 0;
  }
}


void DACInit( void )
{
  // Configuramos el pin p0.26 como salida DAC 
	// En PINSEL1 bits 20 a 0 y bit 21 a 1
  LPC_PINCON->PINSEL1 &= ~(0x1<<20);
	LPC_PINCON->PINSEL1 |= (0x1<<21);
	
  return;
}