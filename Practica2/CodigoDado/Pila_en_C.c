//declaración variables globales
int suma; // sin inicializar
int dato; // sin inicializar

//declaración funciones
int Suma(int a, int b);
int Multiplica(int a, int b, int c);


int main(void)
{
 dato=Multiplica(5, 8, 10);

 while(1);
}

// función que suma a+b 
int Suma(int a, int b)
{
 int temp;	// variable temporal
 temp= a+b;
 return temp;
}
 
// función que calcula dato= c*(a+b) 
int Multiplica(int a, int b, int c)
{
 int temp_suma;	// variable temporal
 int temp;	    // variable temporal
 temp_suma = Suma(a,b);	   // Lama a la función suma
 temp= c*temp_suma;
 return temp;
}

