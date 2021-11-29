  ;
  ;    Programa para el estudio de los modos 
  ;	   de direccionamiento en ensamblador

STACK_TOP EQU 0x10004000 

     AREA RESET, CODE ; La directiva ÁREA indica al 
	 				  ; ensamblador crear una nueva sección 
					  ; de código o de datos. 
	                  ; Las secciones son independientes 
					  ; e indivisibles para el linker
	 DCD STACK_TOP    ;	La directiva DCD asigna 
	 				  ; una o más palabras de memoria,
	                  ; alineados en grupos de cuatro bytes, 
					  ; y define el contenido inicial de la memoria.
     DCD inicio
	 
	 SPACE (0x400-8)  ; Dejamos libre el primer Kbyte de memoria
					  ; El programa comenzará en la dirección 0x400
	 
	 ENTRY 	 
	  
inicio 		          ;  Inicio del programa principal
   
		MOV r0,#9 
		MOV r1,#0  
		ADD r1,#4
		MOV r2,#0xA
		MOV r5,#0x10000000  
		MOV r4,r5 

		STR r0,[r5]         
		LDR r3,[r5]         
		STR r0,[r5,#4]      
		LDR r3,[r5,#4]!     

		STR r1,[r5,r1]      
		LDR r3,[r5,r1]

		STR r4,[r5],#2
		
		LDMIA r4,{r0-r2}
		
		LDR r0,=Lista
		LDR r2,=Num
		LDRB r1,[r2]
		LDR r2,=Suma
		MOV r3,#0x00
		MOV r5,#0x4

lazo  ;for(i = NUM; BNE "Breach if not equual"; i--)
		LDR r4,[r0]  
		ADD r0,r5 		
		ADD r3,r4	;Actualizo R3 con el valor de R4
		SUBS r1,#1  ;i-- en C                          #RI-= 1
		BNE lazo  ;condicional de cerrar bucle 
		
		STR r3,[r2]

deadloop  
		B deadloop  
 
		AREA Constantes, DATA, READONLY
Lista	DCD 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A
Num		DCB 10

		AREA Variables, DATA
Espacio	SPACE 40
Suma	SPACE 4
Numeros SPACE 80

    END  

 