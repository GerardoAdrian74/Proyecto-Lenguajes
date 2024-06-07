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

vector<Constante> CargarConstantes() {
	ifstream archivo;
	archivo.open("Constantes.txt", ios::in);
	if (archivo.fail()) {
		cout << "Error abriendo el archivo de constantes\n";
		archivo.close();
	}
	vector<Constante> arr_Constantes;
	
	string linea;
	
	while (getline(archivo, linea)) {
		bool claveLeida = false;
		string  tmpclave = "", tmpValor = "";
		for (int i = 0; i < linea.size(); i++) {
			if (linea[i] != ' ') {
				if (claveLeida == true) {
					tmpValor += linea[i];
				}
				if (linea[i] == '=') {
					claveLeida = true;
				}
				if (claveLeida == false) {
					tmpclave += linea[i];
				}
			}

		}
		Constante constante;
		constante.clave = tmpclave;
		constante.valor = stof(tmpValor);
		arr_Constantes.push_back(constante);

	}
	archivo.close();

	return arr_Constantes;
}

int main(int argc, char* args[]) {
	ifstream archivo;
	archivo.open("Constantes.txt", ios::in);
	if (archivo.fail()) {
		cout << "Error abriendo el archivo de constantes\n";
		archivo.close();
		return 1;
	}
	vector<Constante> Lista_Constantes= CargarConstantes();
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			//Update the surface
			SDL_UpdateWindowSurface(window);

			//Hack to get window to stay up
			SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
		}
	}
	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}