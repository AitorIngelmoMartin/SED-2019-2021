/* main.c */
#include "delay.h"
#include "lpc17xx.h"
 
void config_GPIO (void)
{
  LPC_GPIO3->FIODIR |= (1<<25);		/* P3.25 definido como salida */ 
  LPC_GPIO2->FIODIR &= ~(1<<12);	/* P2.12 definido como entrada */ 
  LPC_GPIO3->FIOSET |= (1<<25);		/* apago el LED conectado a P3.25 */
}
     
  int main (void)
{ 
  
	config_GPIO();
	while(1) {

  if ((LPC_GPIO2->FIOPIN & (1<<12))==0){  /* pulsador pulsado? */
    LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
    delay_1ms (200);			        /* retardo de 200ms */
    LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
    delay_1ms (400);					/* retardo de 400ms */
    }
  else {
		LPC_GPIO3->FIOPIN |= (1<<25);		/* apago el LED */
    delay_1ms (100);					/* retardo de 100ms */
    LPC_GPIO3->FIOPIN &= ~(1<<25);		/* enciendo el LED */
    delay_1ms (200);					/* retardo de 200ms */
    } 
  }
}



