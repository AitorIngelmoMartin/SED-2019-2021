/*----------------------------------------------------------------------------
 * Nombre:    Timer1_Ejemplo.c 
 *----------------------------------------------------------------------------*/

#include "LPC17xx.H"	   
void inicializa (void){
  LPC_GPIO3->FIODIR |= (1<<26); // P3.268 definido como salida
  LPC_SC->PCLKSEL0 |= 1 << 5; // Clock para el Timer1 = CCLK/2
	
   LPC_TIM1->MR0 = 150-1; // Damos un valor apropiado para la frecuencia de
                         // parpadeo del LED basándonos en la frecuencia del reloj.
   LPC_TIM1->MR1 = 500-1;
   LPC_TIM1->MCR |=0x19; //Valores 0 1 1 para timer1 y 001 para timer0
                                                   // CONFIGURO:Interrumpir cuando se alcance Match0 (bit0) y resetear el contador del Timer
                                                   // cuando alcance el valor del Match0 (bit1) (Tabla 429, página 496)
	
	
  LPC_TIM1->PR = 50e3 -1;	 // Cada vez que el Prescaler alcance este valor se incrementa en uno el contador del Timer
	
	
  LPC_TIM1->TCR |= 1 << 1; // Resetear Timer (Tabla 427, página 494)
  LPC_TIM1->TCR &= ~(1<<1);     // Volvemos a poner a 0 para eliminar el Reset de la línea anterior
  NVIC_EnableIRQ(TIMER1_IRQn); // Habilitar interrupción Timer
  LPC_TIM1->TCR |= 1 << 0;    // Arrancar el Timer (Tabla 427, página 494)
	

}

void TIMER1_IRQHandler (void)
{
    if((LPC_TIM1->IR & 0x01) != 0) // Si se ha interrumpido por MR0
    {
        LPC_TIM1->IR |= 1 << 0; // Borrar flag de interrupción MR0
        LPC_GPIO3->FIOPIN ^= 1 << 26; // Conmutar estado del LED2
    }
		if((LPC_TIM0->IR & 0x02) != 0) // Si se ha interrumpido por MR0
		{
				LPC_TIM0->IR |= 1 << 0; // Borrar flag de interrupción MR0
				LPC_GPIO3->FIOPIN ^= 1 << 26; // Conmutar estado del LED2
		}
		
}


int main (void)
{
  inicializa();
  while(1);
}
