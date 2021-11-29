#include <LPC17xx.H>
#include <string.h>
#include <stdio.h>

#define true 1
#define false 0	
	
       //Temperatura media medida
			 int8_t temperaturaMediaMedida =0;
			 int8_t errorEnMedia=false;
      
       //Multiplexación
			 int8_t conmutador=0;

			 //Variables para el modo programador
			 int8_t temperaturaProgramada= 12;
			 int8_t mododeprograma=0;
			 int8_t temperaturaPredefinida = 12;
			 
			 //Variables Timer
			 int8_t hora = 9;
			 int8_t horaFija=false;
			 int8_t ticks=0;
			 
			 //Sensores
			 int8_t temperaturaSensor1=0;
			 int8_t temperaturaSensor2=0;
			 
			 //Representación 
		  int8_t unidades=0;
			int8_t decenas= 0;
			int8_t valorDelPin1 =0;
			int8_t valorDelPin2 =0;
			 
			//Displays
			int8_t SignoMenos= 0x40;
			int8_t LetraA =  0x77;
			int8_t LetraL =  0x38;
			
			uint16_t Disp[10] ={0x3F,  //0
				 									0x06,   //1 
													0x5B ,  //2
													0x4F ,  //3
													0x66 ,  //4
													0x6D ,  //5
													0x7C ,  //6
													0x07 ,  //7
													0x7F ,  //8
													0x67};	//9


void inicioHardware(){

//Configuracion de los pines P2.10 a P2.12, como entradas de interrupcion
 LPC_PINCON->PINSEL4 |= 1 << (10*2);   //ISP para el modo de programacion
 LPC_PINCON->PINSEL4 |= 1 << (11*2);   //KEY1 para el "+"
 LPC_PINCON->PINSEL4 |= 1 << (12*2);   //KEY2 para el "-"	

	//----Pines AO/A1 multiplexación
		LPC_PINCON->PINSEL4 &= ~(0x3 << 3);  // AO
		LPC_PINCON->PINMODE4 &= ~(0x01 << 3); // Sin resistencia interna

		LPC_PINCON->PINSEL4 &= ~(0x3 << 4);  // A1
		LPC_PINCON->PINMODE4 &= ~(0x01 << 4); // Sin resistencia interna
	//----
	
	//----led modo prog
		LPC_PINCON->PINSEL4 &= ~(0x3 << 6);  // Función GPIO
		LPC_PINCON->PINMODE4 &= ~(0x3 << 6); // Resistencia pull-up
	  LPC_GPIO2->FIODIR &= ~(0x0 << 6);    // bit P2.06 definido como entrada
	//----
	
	//----led caliente
		LPC_PINCON->PINSEL4 &= ~(0x3 << 1);  // Función GPIO
		LPC_PINCON->PINMODE4 &= ~(0x3 << 1); // Resistencia pull-up
		LPC_GPIO2->FIODIR &= ~(0x0 << 1);    // bit P2.06 definido como entrada
	//----

	//----led modo frio
		LPC_PINCON->PINSEL4 &= ~(0x3);  // Función GPIO
		LPC_PINCON->PINMODE4 &= ~(0x3); // Resistencia pull-up
		LPC_GPIO2->FIODIR &= ~(0x0);    // bit P2.06 definido como entrada
		
	//----Entradas y salidas  
	LPC_GPIO2->FIODIR =((0x01)|(0x01<<1)|(0x01<<3)|(0x01<<4)|(0x00<<6));
	//LPC_GPIO2->FIOCLR = (0x01<<6); 
	
 LPC_GPIO2->FIODIR &= ~(1<<10); /* ISP definido como entrada */
 LPC_GPIO2->FIODIR &= ~(1<<11); /* Key_1 definido como entrada */
 LPC_GPIO2->FIODIR &= ~(1<<12); /* Key_2 definido como entrada */
	
//P1.18-P1.24 como salidas para los displays
  LPC_GPIO1->FIODIR = (0x7F)<<18;              
}

void configuracionDeInterrupcionesYtimer1(){
	
//Asignacion de prioridades
  NVIC_SetPriorityGrouping(3);	
	NVIC_SetPriority(TIMER0_IRQn, 0x0);//Timer0
	NVIC_SetPriority(EINT0_IRQn, 0x20);//BOTONES
  NVIC_SetPriority(EINT1_IRQn, 0x40);
  NVIC_SetPriority(EINT2_IRQn, 0x40);

//Habilitacion de las interrupciones
	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
	
//Configurar EXTMODE y EXTPOLAR 0
	LPC_SC->EXTMODE |= (1<<0); 
	LPC_SC->EXTPOLAR |=(0<<0);	
	
	LPC_SC->EXTMODE |= (1<<1);
	LPC_SC->EXTPOLAR |=(0<<1);
	
	LPC_SC->EXTMODE |= (1<<2); 
	LPC_SC->EXTPOLAR &=~(0<<2);
	
//Configuracion timer	
  LPC_SC->PCLKSEL1 |= 0 << 1;    // Clock para el Timer0 = CCLK()
  LPC_TIM1->MR0 =1;              // Cuando cuente una vez hasta PR interrumpe.  
  LPC_TIM1->MR1 =0x100000; 
  LPC_TIM1->MCR =((0x01)|(0x03<<3));           // CONFIGURO:Interrumpir cuando se alcance Match0 (bit0) y resetear el contador del Timer		
  LPC_TIM1->PR =0;	           // T=(150000000*20ns=3seg)|Cada T segundos se incrementa en 1 el preescaler
  LPC_TIM1->TCR =0x01;           
}
void Configuracion_Systick(void){
SysTick->LOAD  = 0x05F5E0FF;                         /* set reload register */
NVIC_SetPriority (SysTick_IRQn, 0x1);               /* set Priority for Systick Interrupt */
SysTick->VAL   = 0;                                /* Load the SysTick Counter Value */
SysTick->CTRL  = 7;                               /* Enable SysTick IRQ and SysTick Timer */
} 
//ISR de las interrupciones
void EINT0_IRQHandler(){ //Rutina para el modo del programa. Entro si pulso el ISP 
		
   	LPC_SC->EXTINT |= (1);   // Borrar el flag de la EINT0 --> EXTINT.0
		
	  if(mododeprograma==1){
				mododeprograma=0;
			  LPC_GPIO2->FIOSET	|= (0x01<<6); 
				LPC_GPIO2->FIOCLR = ((0x01<<1)||(0x01));
		}else{
				mododeprograma=1;
			LPC_GPIO2->FIOCLR	|= (0x01<<6); 	
			LPC_GPIO2->FIODIR |= (0x01<<6); 
		}			  
}
//Rutina KEY1
void EINT1_IRQHandler(){//Rutina para aumentar la temperatura

	    LPC_SC->EXTINT |= (1 << 1);                   // Borrar el flag de la EINT1 --> EXTINT.1	
	    if(mododeprograma==1){
	      temperaturaProgramada++;
		    
				if((temperaturaProgramada>35)){             //Sistema para no salirme del rango
		       temperaturaProgramada=35;
	      }
	    }
}

//Rutina KEY2
void EINT2_IRQHandler(){//Rutina para disminuir la temperatura
	
     		LPC_SC->EXTINT |= (1 << 2);                 // Borrar el flag de la EINT2 --> EXTINT.2	
				if(mododeprograma==1){
					temperaturaProgramada--;
					if(temperaturaProgramada<(-9)){             //Sistema para no salirme del rango
						temperaturaProgramada=(-9);
					}
				}
}
void leerSensores(void){
				//Leo los sensores 1 y 2
				temperaturaSensor1=(LPC_GPIO0->FIOPIN &= (0x0F));    // Leo los P0.00-04 [SENSOR 1]
				temperaturaSensor2=(LPC_GPIO0->FIOPIN >> 4) & 0x0F ; // Leo los P0.05-08 [SENSOR 2]
				
}
//FUNCION para verificar si hay errores en los sensores
void falloEnMedidas(void){
	
	  int8_t media=0;
		int8_t temperaturaObtenida=0;
		
   	if(temperaturaSensor1<0 && temperaturaSensor2<0){																		//SOLUCIONAR SI ES NEGATIVo
				temperaturaSensor1*= (-1);
				temperaturaSensor2*= (-1);
		}
			
		media = (temperaturaSensor1+temperaturaSensor2)/2;
	
		if((temperaturaSensor1 - temperaturaSensor2)>=3 || (temperaturaSensor2-temperaturaSensor1)>=3){
					errorEnMedia= true;							
		}else{
				errorEnMedia = false;	
		}
		  temperaturaObtenida = (50/15)*media-15;
			temperaturaMediaMedida = temperaturaObtenida;
}
void multiplexacion(char tarea){  

//Representar negativos
  if(tarea==0){
					if(conmutador==0){
				 //Represento decenas
					 LPC_GPIO2->FIOCLR = (0X01<<3);
					 LPC_GPIO2->FIOSET = (0X01<<4);		
					 LPC_GPIO1->FIOCLR= 0xFFFFFFFF;
					 valorDelPin2=SignoMenos ;					
					 LPC_GPIO1->FIOPIN = (valorDelPin2)<<18;
					 conmutador=1;
				}else{
					 //Represento unidades			
					 LPC_GPIO2->FIOSET = (0X01<<3);		
					 LPC_GPIO2->FIOCLR = (0X01<<4);		
					 LPC_GPIO1->FIOCLR = 0xFFFFFFFF;
					 valorDelPin2=Disp[unidades] ; 						
					 LPC_GPIO1->FIOPIN = (valorDelPin2)<<18;				
					 conmutador=0;
				}			
	}
//Represenat pisitivos	
	if(tarea==1){
					if(conmutador==0){
				 //Represento decenas
					 LPC_GPIO2->FIOCLR = (0X01<<3);
					 LPC_GPIO2->FIOSET = (0X01<<4);		
					 LPC_GPIO1->FIOCLR= 0xFFFFFFFF;
					 valorDelPin2=Disp[decenas] ;					
					 LPC_GPIO1->FIOPIN = (valorDelPin2)<<18;
					 conmutador=1;
				}else{
					 //Represento unidades			
					 LPC_GPIO2->FIOSET = (0X01<<3);		
					 LPC_GPIO2->FIOCLR = (0X01<<4);		
					 LPC_GPIO1->FIOCLR = 0xFFFFFFFF;
					 valorDelPin2=Disp[unidades] ; 						
					 LPC_GPIO1->FIOPIN = (valorDelPin2)<<18;				
					 conmutador=0;
				}			
	}	
//Fuera de rango
  if(tarea==2){
					if(conmutador==0){
				 //Represento decenas
					 LPC_GPIO2->FIOCLR = (0X01<<3);
					 LPC_GPIO2->FIOSET = (0X01<<4);		
					 LPC_GPIO1->FIOCLR= 0xFFFFFFFF;
					 valorDelPin2=LetraA;					
					 LPC_GPIO1->FIOPIN = (valorDelPin2)<<18;
					 conmutador=1;
				}else{
					 //Represento unidades			
					 LPC_GPIO2->FIOSET = (0X01<<3);		
					 LPC_GPIO2->FIOCLR = (0X01<<4);		
					 LPC_GPIO1->FIOCLR = 0xFFFFFFFF;
					 valorDelPin2=LetraL ; 						
					 LPC_GPIO1->FIOPIN = (valorDelPin2)<<18;				
					 conmutador=0;
				}		
	}
//Representar Tª fijada	
  if(tarea==3){
					if(conmutador==0){
				 //Represento decenas
					 LPC_GPIO2->FIOCLR = (0X01<<3);
					 LPC_GPIO2->FIOSET = (0X01<<4);		
					 LPC_GPIO1->FIOCLR= 0xFFFFFFFF;
					 valorDelPin2=Disp[1] ;					
					 LPC_GPIO1->FIOPIN = (valorDelPin2)<<18;
					 conmutador=1;
				}else{
					 //Represento unidades			
					 LPC_GPIO2->FIOSET = (0X01<<3);		
					 LPC_GPIO2->FIOCLR = (0X01<<4);		
					 LPC_GPIO1->FIOCLR = 0xFFFFFFFF;
					 valorDelPin2=Disp[2] ; 						
					 LPC_GPIO1->FIOPIN = (valorDelPin2)<<18;				
					 conmutador=0;
				}	
	}		
}
void representarTemperatura(int8_t modo){
	if(temperaturaProgramada==temperaturaMediaMedida){
								//Apago leds caliente/frío	
									LPC_GPIO2->FIOSET	= ((0x01)|(0x01<<1));		
			}else{						
					if(temperaturaProgramada>temperaturaMediaMedida){
							//Led de calentado
									LPC_GPIO2->FIOSET	= (0x01);
									LPC_GPIO2->FIOCLR = (0x01<<1);     				
					}else{
							//Led de enfriado
									 LPC_GPIO2->FIOSET	= (0x01<<1);
									 LPC_GPIO2->FIOCLR =  (0x01);       
					}
			}	
if(modo==0){
	if(temperaturaMediaMedida>0){                                               //Obtención de decenas y unidades medidas			
						if((temperaturaMediaMedida - 10)>=0){                                     
							 decenas  = (temperaturaMediaMedida/10);
							 unidades =(temperaturaMediaMedida-(decenas*10));
						}else{                                                         			
							 decenas=0;	
							 unidades=temperaturaMediaMedida;
						}	 					
		    multiplexacion(1);
				}else{																													//Representación de valor medido negativo			
				multiplexacion(0);										
				}	
}			

if(modo==1){
	if(temperaturaProgramada>=0){ //Representación temperatura programada a mano  								
						if((temperaturaProgramada - 10)>=0){                         
								decenas  = (temperaturaProgramada/10);
								unidades =(temperaturaProgramada-(decenas*10));
						}else{                                                       
								decenas=0;	
								unidades=temperaturaProgramada;
						}
						multiplexacion(1);
			}else{ //Representación valor programado es negativo	
				unidades=(-temperaturaProgramada);
				multiplexacion(0);
			}				
			}
				
}
//Inicio main
int main (void){	
				inicioHardware();
				configuracionDeInterrupcionesYtimer1();
				Configuracion_Systick();
				while(1){}		
}
void SysTick_Handler (void){ // ISR del SYSTICK-	
		ticks++;
		if(ticks==60){
					hora++;
					if(hora>24){
					hora=0;
			   	}	
				
				if((hora>=22) || (hora<=8)){//Si estoy entre la 22:00 y las 8:00
					horaFija = true;
				}else{
					horaFija = false;
				}			
		ticks=0;
		} 
}
void TIMER1_IRQHandler(void){
	
	LPC_TIM1->IR = 0x1<<0;   	//Borro el flag del Match0	
	if(LPC_TIM1->MR1==LPC_TIM1->TC){
							//Leo los sensores
							leerSensores();
						//Compruebo si ha habido un error de medicion
							falloEnMedidas();		
	}
	if(horaFija==false){	
				if(mododeprograma==0){//Modo "normal"
							if(errorEnMedia==false){//Si no hay error, establezco la temperatura
							representarTemperatura(0);
							}else{
								//Represento AL	
								LPC_GPIO2->FIOSET =((0x01)|(0x01<<1)|(0x01<<6));
								multiplexacion(2);
							}			
				}else{				
					representarTemperatura(1);		
				}		
 }else{
								LPC_GPIO2->FIOSET =((0x01)|(0x01<<1)|(0x01<<6)); 
	 //Temperatura a 12ºC
   multiplexacion(3);
 }	

}