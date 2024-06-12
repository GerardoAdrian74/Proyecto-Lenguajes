#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;
class Constantes_Variables {
public:
	struct Constante {
		string clave;
		float valor;

	};


	bool esNumeroValido(const string& s) {
		char* end = nullptr;
		errno = 0;
		float val = std::strtof(s.c_str(), &end);
		return end == s.c_str() + s.size() && errno == 0;
	}
	bool SoloLetras(const string& s) {
		return all_of(s.begin(), s.end(), ::isalpha);
	}
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
		if (esNumeroValido(tmpValor) && SoloLetras(tmpclave) && tmpclave != "" && tmpValor != "") {
			Exp_Parseado.valor = stof(tmpValor);
			Exp_Parseado.clave = tmpclave;
		}
		else {
			Exp_Parseado.valor = -1;
			Exp_Parseado.clave = '/';
		}
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
			Constante constante = parsearExpresion(linea);
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
	int agregarVariable(vector<Constante> ListaConstantes, vector<Constante> ListaVariables, string expresion) {
		ifstream leer;
		ofstream escribir;

		leer.open("Variables.txt");
		if (leer.fail()) {
			leer.close();
			escribir.open("Variables.txt", ios::out);

		}
		else {
			escribir.open("Variables.txt", ios::app);
		}
		//Parsear expresion
		Constante ExpresionParseada = parsearExpresion(expresion);
		if (ExpresionParseada.valor == -1 && ExpresionParseada.clave == "/") {
			return -1;
		}
		//Comparacion Constantes
		for (Constante tmp : ListaConstantes) {
			if (ExpresionParseada.clave == tmp.clave) {
				return 1;
			}
		}
		//Comparacion Variables
		for (Constante tmp : ListaVariables) {
			if (ExpresionParseada.clave == tmp.clave) {
				return 2;
			}
		}
		escribir << expresion << endl;
		escribir.close();

		return 0;
	}
	void modificarVariable(string stringexpresion) {
		ifstream leer;
		ofstream escribir;
		string linea;
		vector<Constante> ListaExpresiones;
		vector<string> arr_string_Expresion;
		Constante expresion;
		expresion = parsearExpresion(stringexpresion);
		ListaExpresiones.push_back(expresion);
		arr_string_Expresion.push_back(stringexpresion);
		leer.open("Variables.txt", ios::in);
		while (getline(leer, linea)) {
			Constante tmp;
			tmp = parsearExpresion(linea);
			if (tmp.clave != expresion.clave) {
				arr_string_Expresion.push_back(linea);
			}
		}
		leer.close();
		escribir.open("Variables.txt", ios::out);
		for (string tmp : arr_string_Expresion) {
			escribir << tmp << endl;
		}
		escribir.close();
	}

};