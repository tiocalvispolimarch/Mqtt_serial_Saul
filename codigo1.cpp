
#include <stdio.h>
#include<iostream>
using namespace std; 

void funcion(); 
typedef struct 
{
	char Msg[30] ="Hello buddy!:)";


}Frase;

typedef struct
{
	int entero=10;
	float fracionario= 5.5;
}Numeros;

typedef struct{
	Frase F;
	Numeros *Num;

}estructuraprincipal;

estructuraprincipal s;
int main()
{

	funcion(); 
cout << s.Num->entero<< endl;

	return 0; 
}

void funcion()
{
	static Numeros man;
	s.Num = &man;
	s.Num->entero = 55; 
}
