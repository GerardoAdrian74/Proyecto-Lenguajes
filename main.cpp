#define CATCH_CONFIG_RUNNER
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <cctype>
#include <algorithm>
#include "catch.hpp"
#include "Constantes_Variables.hpp"
using namespace std;

int runTests() {
	char* argv[] = { (char*)"", (char*)"--success" }; 
	int argc = 2;
	return Catch::Session().run(argc, argv);
}

TEST_CASE("Test Evaluador de Expresiones", "[Constantes_Variables]") {
	Constantes_Variables obj;

	SECTION("Prueba esNumeroValido") {
		REQUIRE(obj.esNumeroValido("12") == true);
		REQUIRE(obj.esNumeroValido("12.34") == true);
		REQUIRE(obj.esNumeroValido("abc") == false);
	}
}

int main(int argc, char* args[]) {
	//Inicio pruebas
	if (false) {
	cout << "Ejecutando pruebas de Catch2..." << endl;
	int testResult = runTests();
	if (testResult != 0) {
		cout << "Algunas pruebas fallaron." << endl;
		return testResult;
	}

	cout << "Todas las pruebas pasaron!" << endl;
	}
	//Fin TDD pruebas
	
	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
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