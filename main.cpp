#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
using namespace std;


struct Constante {
	string clave;
	float valor;

};
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Constante parsearExpresion(string expresion) {
	Constante Exp_Parseado;
	bool claveLeida = false;
	string  tmpclave = "", tmpValor = "";
	for (int i = 0; i < expresion.size(); i++) {
		if (expresion[i] != ' ') {
			if (claveLeida == true) {
				tmpValor += expresion[i];
			}
			if (expresion[i] == '=') {
				claveLeida = true;
			}
			if (claveLeida == false) {
				tmpclave += expresion[i];
			}
		}

	}
	Exp_Parseado.clave = tmpclave;
	Exp_Parseado.valor = stof(tmpValor);

	return Exp_Parseado;
}
vector<Constante> CargarConstantes() {
	ifstream archivo;
	vector<Constante> arr_Constantes;
	archivo.open("Constantes.txt", ios::in);
	if (archivo.fail()) {
		archivo.close();
		return arr_Constantes;
	}
	
	
	string linea;
	
	while (getline(archivo, linea)) {
		Constante constante=parsearExpresion(linea);
		arr_Constantes.push_back(constante);

	}
	archivo.close();
	return arr_Constantes;
}
vector<Constante> CargarVariables() {
	ifstream archivo;
	vector<Constante> arr_variables;
	archivo.open("Variables.txt", ios::in);
	if (archivo.fail()) {
		archivo.close();
		return arr_variables;
	}
	string linea;
	while (getline(archivo, linea)) {
		Constante constante = parsearExpresion(linea);
		arr_variables.push_back(constante);

	}
	archivo.close();
	return arr_variables;
}

bool agregarVariable(vector<Constante> ListaConstantes, vector<Constante> ListaVariables,string expresion) {
		ifstream leer;
		ofstream escribir;

		leer.open("Variables.txt");
		if (leer.fail()) {
			leer.close();
			escribir.open("Variables.txt",ios::out);
			
		}
		else {
			escribir.open("Variables.txt", ios::app);
		}	
		//Parsear expresion
		Constante ExpresionParseada=parsearExpresion(expresion);
		//Comparacion Constantes
		for (Constante tmp : ListaConstantes) {
			if (ExpresionParseada.clave==tmp.clave) {
				return false;
			}
		}
		//Comparacion Variables
		for (Constante tmp : ListaVariables) {
			if (ExpresionParseada.clave == tmp.clave) {
				return false;
			}
		}
		escribir <<expresion<<endl;
		escribir.close();
		
	return true;
}
int main(int argc, char* args[]) {
	ifstream archivo;
	vector<Constante>  Lista_Constantes = CargarConstantes();
	vector<Constante> Lista_Variables = CargarVariables();
	string expresion;
	cout << "Ingresa una expresion para una variable: ";
	cin >> expresion;
	if(agregarVariable(Lista_Constantes, Lista_Variables, expresion)) {
		cout << "Se agrego correctamente\n";
	}
	else {
		cout << "Error en la variable\n";
	}

	//SDL
	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = NULL;

	//Inicializar SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		//Crear ventana
		window = SDL_CreateWindow("Evaluador de Expresiones", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			bool running = true;
			while (running) {
				SDL_Event event;
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						running = false;
					}
				}
			}
		}

		//Salir de la ventana
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 0;
	}
}