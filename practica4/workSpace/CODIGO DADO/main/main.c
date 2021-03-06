//controlador de interrupciones


#include <LPC17xx.H>
#include <string.h>
#include <stdio.h>

//char a=1, b=1, c=1;
       char interrupcion_0 =0;
       char interrupcion_1 =0;


//Funci?n que inicializa las interrupciones
void inicioPINSEL()
{

//Configuraci?n de los pines P2.10 a P2.12, como entradas de interrupci?n
 LPC_PINCON->PINSEL4 |= 1 << (10*2);   
 LPC_PINCON->PINSEL4 |= 1 << (11*2);   
 LPC_PINCON->PINSEL4 |= 1 << (12*2);   
 
	
	//-----------------------------------------------------------------------
//Configurar P0.04/06/08 como salidas
	  LPC_GPIO0->FIODIR |= (0x1<<4); // P0.04 definido como salida 
		LPC_GPIO0->FIODIR |= (0x1<<6); // P0.06 definido como salida 
		LPC_GPIO0->FIODIR |= (0x1<<8); // P0.08 definido como salida 
	
	  //******** RESISTENCIAS PULL-UP
	
	// siempre que x sea 0 a 15, en este caso ser?a  x=5 , 7,   9 . 
LPC_PINCON->PINSEL0 &= ~(0x3 << 5); // Funci?n GPIO
LPC_PINCON->PINMODE0 &= ~(0x3 << 5); // Resistencia pull-up
LPC_GPIO0->FIODIR &= ~(0x1 << 5); // bit P0.5 definido como entrada
	
LPC_PINCON->PINSEL0 &= ~(0x3 << 7); // Funci?n GPIO
LPC_PINCON->PINMODE0 &= ~(0x3 << 7); // Resistencia pull-up
LPC_GPIO0->FIODIR &= ~(0x1 << 7); // bit P0.7 definido como entrada

LPC_PINCON->PINSEL0 &= ~(0x3 << 9); // Funci?n GPIO
LPC_PINCON->PINMODE0 &= ~(0x3 << 9); // Resistencia pull-up
LPC_GPIO0->FIODIR &= ~(0x1 << 9); // bit P0.9 definido como entrada
	  //********
		
		
	//-----------------------------------------------------------------------------------
}

void inicioIRQs()
{
	//Configurar EXTMODE y EXTPOLAR , MODE => 0 Nivel 1 Flanco
	LPC_SC->EXTMODE |= (1<<0);
	LPC_SC->EXTPOLAR |=(1<<0);
	
	LPC_SC->EXTMODE |= (1<<1);//por flanco 
	LPC_SC->EXTPOLAR &=~(1<<1);//0,0 descendente
	//-----------------------------
	
	
//Asignaci?n de prioridades
  NVIC_SetPriorityGrouping(5);
  NVIC_SetPriority(EINT0_IRQn, 0x04);
  NVIC_SetPriority(EINT1_IRQn, 0x06);
  NVIC_SetPriority(EINT2_IRQn, 0x02);

//Habilitaci?n de las interrupciones
  NVIC_EnableIRQ(EINT0_IRQn);
  NVIC_EnableIRQ(EINT1_IRQn);
  NVIC_EnableIRQ(EINT2_IRQn);
}

//ISR de las interrupciones
void EINT0_IRQHandler(){ 
				LPC_SC->EXTINT |= (1);   // Borrar el flag de la EINT0 --> EXTINT.0
	      LPC_GPIO0->FIOSET = 0x00000100;     // Activo el P0.04
				LPC_GPIO0->FIOCLR = 0x00000040;     // Activo el P0.06
	
				interrupcion_0 ++;
				//printf("Interrupciones 0: %d\n",interrupcion_0);
				//LPC_GPIO0->FIOCLR = 0x00000010;     // Desactivo el P0.04
				
}

void EINT1_IRQHandler()
{
	LPC_SC->EXTINT |= (1 << 1);   // Borrar el flag de la EINT1 --> EXTINT.1	
	      LPC_GPIO0->FIOSET = 0x00000040;     // Activo el P0.06
			
				LPC_GPIO0->FIOCLR = 0x000000100;     // Desactivo el P0.06
			interrupcion_1 ++;
			//printf("Interrupciones 1: %d\n",interrupcion_1);
	}

void EINT2_IRQHandler()
{
			
				//LPC_SC->EXTINT |= (1 << 2);   // Borrar el flag de la EINT2 --> EXTINT.2	
				//LPC_GPIO0->FIOSET = 0x00000100;     // Activo el P0.08
				//while((LPC_GPIO0->FIOPIN & (1<<9)) != 0);
				//LPC_GPIO0->FIOCLR = 0x00000100;     // Desactivo el P0.08
			
}

//Programa principal
int main (void)
{
  inicioPINSEL();

  inicioIRQs();

  while(1){		
					
  }	
		
 }

	//do{
	
//	}while((LPC_GPIO0->FIOPIN & 0x03));
