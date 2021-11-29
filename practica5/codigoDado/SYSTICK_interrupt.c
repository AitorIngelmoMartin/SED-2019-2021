/*----------------------------------------------------------------------------
 * Name:    SYSTIC_interrupt.c
 *----------------------------------------------------------------------------*/

#include "LPC17xx.H"  
uint32_t pulsador;
uint32_t SystemFrequency=100000000;

volatile uint32_t cuenta;  // Contador de veces de x ms


void SysTick_Handler (void) // Rutina (ISR) de atención a la excepción 15 -del SYSTICK-
                                    
{
  cuenta ++;                 // Incrementar countadores cada 10ms  y cada 1s
}



void Configuracion_Systick(uint32_t prioridad, uint32_t ticks)
{
SysTick->LOAD  = (ticks-1) ;                         /* set reload register */
NVIC_SetPriority (SysTick_IRQn, prioridad);         /* set Priority for Systick Interrupt */
SysTick->VAL   = 0;                                /* Load the SysTick Counter Value */
SysTick->CTRL  = 7;                               /* Enable SysTick IRQ and SysTick Timer */
return ;                                         /* Function successful */
}
int main (void)
  {
  LPC_GPIO3->FIODIR |= (1<<25);  // P3.25 definido como salida
  LPC_GPIO3->FIODIR |= (1<<26);  // P3.26 definido como salida  
  LPC_GPIO2->FIODIR &= ~(1<<12);  // P2.12 definido como entrada  
 

Configuracion_Systick(15, 2000000);

  while (1)
{
	
	
if(SysTick->CTRL & (1<<16)){	
	
	cuenta++;

} 
	
    pulsador=((LPC_GPIO2->FIOPIN & (1<<12))>>12);  
    if (pulsador==0){               // pulsador pulsado?
   
      if (cuenta==125)
{
       LPC_GPIO3->FIOPIN &= ~(1<<26); // enciendo LED 3.26
       LPC_GPIO3->FIOPIN   |= (1<<25); } // apago 3.25
       if (cuenta>=125*2){
       LPC_GPIO3->FIOPIN   &= ~(1<<25);        
       LPC_GPIO3->FIOPIN   |= (1<<26); 
       cuenta=0;
          }
       }
    else{
     
       if (cuenta==25) {
       LPC_GPIO3->FIOPIN &= ~(1<<26); 
       LPC_GPIO3->FIOPIN   |= (1<<25);
}
       if (cuenta>=25*2){
          LPC_GPIO3->FIOPIN |= (1<<26);              
          LPC_GPIO3->FIOPIN   &= ~(1<<25);
     cuenta=0;
          }
    }
		
  }

}
