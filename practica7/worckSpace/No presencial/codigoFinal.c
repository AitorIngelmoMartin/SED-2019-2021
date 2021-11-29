#include "lpc17xx.h"
#include "math.h"
#define DAC_BIAS 0x00010000
#define NUM_MUESTRAS 50
#define PI 3.1416


uint16_t Ondasenoidal [NUM_MUESTRAS];
uint16_t Contadormuestras=0, FrecuenciaKhz=1;
uint16_t Frecuencia;
uint16_t freq, i;


void DACInit(void);
void inicializacionArray(void);
void ConfiguraGPIO(void);
void ConfiguraTimer0(void);
void ConfiguraInterrupcion(void);
uint16_t LeerFrecuencia (void);
uint16_t dibujo=0;

void DACInit( void ){
 
  LPC_PINCON->PINSEL1 &= ~(0x1<<20);
  LPC_PINCON->PINSEL1 |= (0x1<<21);
	
  return;
}


void inicializacionArray(void){

  for(i=0;i<NUM_MUESTRAS;i++){
  	Ondasenoidal[i] = 512 + 512*sin(2*PI*i/NUM_MUESTRAS);	//Ponemos 512 porque el DAC es de 10 bits (1023 valores distintos) y justo en 512 sería la mitad de 1023
	  
	/*if(i<=25)
  		Ondasenoidal[i]= 1023;
  	else
    
		Ondasenoidal[i]= 0;	
	*/
    }
}


void ConfiguraGPIO(void)
{ 
	LPC_PINCON->PINSEL3 &=~0xFC00;// Pines 21-24 GPIO (1111 1100 0000 0000)
	
	LPC_PINCON->PINMODE3 &=~ 0xFC00;  //(1111 1100 0000 0000)

	LPC_GPIO1->FIODIR &=~0xE00000;    //1110 0000 0000 0000 0000 Puestos como pull-up
	
	LPC_PINCON->PINSEL4 |= 1<<(12*2);
}

void ConfiguraTimer0(void){

	LPC_SC->PCLKSEL0 |= 1<<5;  // Clock para Timer0 = CCLK/2 (0Mhz)

	//Establecer el Match para el DAC 
	LPC_TIM0->MR0= (50e6/FrecuenciaKhz/NUM_MUESTRAS)-1; //Va en función de "FrecuenciaKhz"
	
	//Interrumpe cuando sea el contador sea igual a March0 
	LPC_TIM0->MCR|=0x3;
	LPC_TIM0->PR=0;//PORQ PONEN  ESTO?

    LPC_TIM0->TCR |= 1 << 1; // Resetear Timer (Tabla 427, página 494)
 	LPC_TIM0->TCR &= ~(1<<1); // Volvemos a poner a 0 para eliminar el Reset de la línea anterior
    NVIC_EnableIRQ(TIMER0_IRQn); // Habilitar interrupción Timer
    LPC_TIM0->TCR |= 1 << 0; // Arrancar el Timer (Tabla 427, página 494)
}

void TIMER0_IRQHandler(void)
{
	//uint8_t i;
	LPC_TIM0->IR |= (1<<0);	//Borro el flag del Match0
	LPC_DAC->DACR = Ondasenoidal[i] << 6;	//Saco una muestra en cada Match
	i++;
	if(i>=50)
	{
	i=0;
	}
}
	
	
/* CODIGO DIFERENTE
     if((LPC_TIM0->IR & 0x01)==0x01){

	     LPC_TIM0->IR |=(1<<0);
	     LPC_DAC->DACR =(Ondasenoidal[Contadormuestras]<<6) | DAC_BIAS;
	     Contadormuestras++;
	  
			 if(Contadormuestras==50){		     
			      Contadormuestras=0;
			   }
    }*/



void EINT2_IRQHandler(void){

	
	LPC_SC->EXTINT |=(1<<2);// borro el flag
	
	FrecuenciaKhz=LeerFrecuencia();

	Frecuencia= (LPC_GPIO1->FIOPIN>>21)& 0xF; // Leo los P1.21-24 

	ConfiguraTimer0();

}


uint16_t LeerFrecuencia (void){

 freq = (Frecuencia+1)*1000;
 return(freq);

}

void ConfiguraInterrupcion(void){

	//Este bloque define que EINT2 tiene un nivel de prioridad mayor.
	NVIC_SetPriorityGrouping(3);
	NVIC_SetPriority(EINT2_IRQn,0x0);
	NVIC_SetPriority(TIMER0_IRQn, 0x2);

	NVIC_EnableIRQ(EINT2_IRQn);// Habilito el Timer y el EINT2 respectivamente
	NVIC_EnableIRQ(TIMER0_IRQn);	
}

int main (void)
{

	//Invoco a todas las funciones
	DACInit();
	inicializacionArray();
	ConfiguraGPIO();
	FrecuenciaKhz=LeerFrecuencia();
	ConfiguraTimer0();
	ConfiguraInterrupcion();


while(1);
}
