#include <iostream>

using namespace std; 
	void funcion1();
typedef struct 
{
	 const char *cadena = "Esto es una cadena ";
}cadenas;//alias nombre que se le pone a la estrutura 

typedef struct 
{
	int variable = 0;
	char datos [10] ;
}datos; 

typedef struct 
{
	/**Aqui hacemos una estrutura añadida*/
	cadenas*  estructura_cadenas_ptr;
	cadenas estructura_cadenas;
	datos* estructura_datos; 
}estructura_principal; 

estructura_principal objeto1;
static datos objeto_chido; 
int main()
{ 
	int a ; 
	while(1)
	{
		cout << objeto1.estructura_cadenas.cadena << endl; 
		funcion1();
		cout << objeto1.estructura_datos->variable << endl;
		cout << objeto_chido.variable << endl;
		break; 
	}
	
	return 0;
}

void funcion1()
{
    objeto1.estructura_datos = &objeto_chido;
	int a ; 
	cout << "Ingrese un numero" << endl;
	cin>> a ;
	objeto1.estructura_datos->variable = a; 
}

