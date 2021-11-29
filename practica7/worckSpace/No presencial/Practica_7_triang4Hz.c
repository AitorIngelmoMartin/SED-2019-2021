#include "lpc17xx.h"

#define DAC_BIAS	0x00010000  // Settling time a valor 2,5us

uint32_t i = 0, val_i;

void DACInit( void )
{
  // setup the p0.26 pin as DAC output 
  // PINSEL1 bits 20 => 0 and bit 21 => 1
  LPC_PINCON->PINSEL1 &= ~(0x1<<20);
  LPC_PINCON->PINSEL1 |= (0x1<<21); 
}


void Configura_SysTick( void )
{
	NVIC_SetPriority  (SysTick_IRQn, 2);  /* set Priority for Systick Interrupt */
	
	SysTick->LOAD  = 1e5 -1;       // Load the SysTick Counter Value   => 1e5 * 10 ns = 1 ms
	SysTick->VAL   = 0;         
  SysTick->CTRL  = (1<<2) | (1<<1) | (1<<0);  // Enable SysTick IRQ and SysTick Timer

	return;
}


void SysTick_Handler (void)			// Rutina (ISR) de atención del SYSTICK
{
	#define MAXi  249
	if ( i > MAXi){  // i va de 0 a 249  => se comienza señal (ciclo) cada 250 * 1ms
		i = 0;
	}
	else{	
		i++;  
	}
	val_i = (i*1023)/MAXi;  // se pasa i a los valores del DAC 0 a 1023, mejor hacer primero la multiplicación para no tener truncamiento
	
	LPC_DAC->DACR = (val_i << 6) | DAC_BIAS;		
	
	return;
}

int main (void)
{
	
  DACInit();   // Inicializa P0.26 como DAC 
  Configura_SysTick();  // configura SysTick para interrumpir cada 10 ms
		
	
  while (1);	
}

