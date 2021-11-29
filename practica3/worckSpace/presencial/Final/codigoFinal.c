
/* main.c */
#include <LPC17xx.H>

//Máscaras para D0 y D1


 
void config_GPIO (void)
{
  LPC_GPIO3->FIODIR |= (1<<25); /* P3.25 definido como salida */
  LPC_GPIO2->FIODIR &= ~(1<<12); /* P2.12 definido como entrada */
  LPC_GPIO0->FIODIR = 0x00000000; /*Pongo los bits 0 y 1 como ENTRADA y los demás como ENTRADA */
  LPC_GPIO3->FIOSET |= (1<<25); /* apago el LED conectado a P3.25 */
}

void delay_1ms(uint32_t ms)
{
    uint32_t i;
    for(i=0;i<ms*14283;i++);
}
  
  

int main (void)
{
	uint32_t valXY;   // EN C SE DEBEN DECLARAR LAS VARIABLES AL PRINCIPIO DE LA FUNCION 
	
	config_GPIO();
	while(1){
		
		
	 
if ((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
	
	valXY=LPC_GPIO0->FIOPIN & 0x03; // si el pin vale 0, valX es 0, si el pin está a 1, valX es distinto de 0
  switch (valXY){
	
		case 0x00:  
			 //20
		
		
	LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
	delay_1ms (800);        /* retardo de 800ms */
	LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
	delay_1ms (200); /* retardo de 200ms */
       break;
		
		case 0x01:  
			 //40
	LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
	delay_1ms (600);        /* retardo de 600ms */
	LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
	delay_1ms (400); /* retardo de 400ms */
      break;
		
		case 0x02:  
			 //60
	LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
	delay_1ms (400);        /* retardo de 400ms */
	LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
	delay_1ms (600); /* retardo de 600ms */
       break;
		
		case 0x03:  
			 //80
	LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
	delay_1ms (200);        /* retardo de 200ms */
	LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
	delay_1ms (800); /* retardo de 800ms */
       break;	
	
	  }
}else{
switch (valXY){
	
		case 0x00:  
			 //20
		
		
	LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
	delay_1ms (800);        /* retardo de 800ms */
	LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
	delay_1ms (200); /* retardo de 200ms */
       break;
		
		case 0x01:  
			 //40
	LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
	delay_1ms (600);        /* retardo de 600ms */
	LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
	delay_1ms (400); /* retardo de 400ms */
      break;
		
		case 0x02:  
			 //60
	LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
	delay_1ms (400);        /* retardo de 400ms */
	LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
	delay_1ms (600); /* retardo de 600ms */
       break;
		
		case 0x03:  
			 //80
	LPC_GPIO3->FIOPIN |= (1<<25);       /* apago LED */
	delay_1ms (200);        /* retardo de 200ms */
	LPC_GPIO3->FIOPIN &= ~(1<<25);      /* enciendo LED */
	delay_1ms (800); /* retardo de 800ms */
       break;	
}
	
	
		
	}//Fin while
	
	
}
	}
	
	//Fin main