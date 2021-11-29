 ; Programa_2.s
STACK_TOP EQU 0x10004000 

		AREA RESET, CODE  
		DCD STACK_TOP    
		DCD inicio
		SPACE (0x400-8) 
	
		ENTRY 	 
	  

inicio MOV r0,#1
	   MOV r1,#31
	   
	   
lazo LSL r0,#1
	 SUBS r1,r1,#1
	 BNE lazo
      

deadloop  
		B deadloop  
 
 		AREA Variables, DATA
Numeros	SPACE 20*4

    END  

 