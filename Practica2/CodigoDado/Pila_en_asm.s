  ;    Programa para el estudio de los modos de direccionamiento en ensamblador

STACK_TOP 	EQU 0x10002000 

			AREA RESET, CODE  
			DCD STACK_TOP     ;puntero de pila inicial
			DCD inicio        ;Rutina de inicio para saltar a 12"inicio"
			SPACE (0x400-8)  
	 	
			ENTRY 	 
	  
inicio 		;  Inicio del programa principal
   
; Se  inicializan algunos registros	para que se pueda observar como se van modificando 
; y los valores que toma la pila con la instrucción PUSH
			LDR R0,=0xEEEEEEEE
			LDR R1,=0xDDDDDDDD
			LDR R2,=0xCCCCCCCC
			LDR R3,=0xBBBBBBBB
			LDR R4,=0xAAAAAAAA
			LDR R5,=0x99999999
			LDR R6,=0x88888888
			LDR R7,=0x77777777
			LDR R8,=0x66666666
; Fin de la inicialización	
									   ;                                   _
			MOVS     r2,#0x0A		   ;	10			                    | 
			MOVS     r1,#0x08		   ;	8			                    |
			MOVS     r0,#0x05		   ;	5			                    |
			BL.W     Multiplica 	   ;llamada a subrrutina "41"			     >  función main
			LDR      r1,=dato 	   	   ;				                    |
			STR      r0,[r1,#0x00]     ;				                    |
while  		B        while	           ; while(1); 		                    |
									   ;                                   -
Suma	 	ADD      r0,r0,r1   
			BX       lr	 ;  return temp; 

 ;// función que calcula dato= c*(a+b)  
 ;int Multiplica(int a, int b, int c) 

Multiplica	PUSH     {lr};como salto aquí en "30", empiezo a registrar en pila
			BL.W     Suma  ;    temp_suma = Suma(a,b);    // Lama a la función Suma , debo preservar LR guardandolo en pila
			MUL      r0,r0,r2	; temp= c*temp_suma; 
			POP      {pc}   ;return temp;   
			
			AREA | Header Data|, DATA
dato		SPACE 4

			END  

 