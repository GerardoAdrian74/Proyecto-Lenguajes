#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include "Constantes_Variables.hpp"

using namespace std;

using namespace std;
class Posfijo
{
public:
    Posfijo();

    int evaluar(const string& expresion);


private:
    string convertirVariable(vector<Constantes_Variables::Constante>& variables, vector<Constantes_Variables::Constante>& constantes, string expresion);
    int tipoOperadores(char op);
    bool es_operador(char c);
    bool validar_expresion(const string& expresion);
    string infija_a_postfija(const string& expresion);
    double evaluar_postfija(const string& expresion);
    int esVariable(vector<Constantes_Variables::Constante>& variables, char expresion);
    int esConstante(vector<Constantes_Variables::Constante>& constantes, char expresion);
    Constantes_Variables* constante = new Constantes_Variables();
    vector<Constantes_Variables::Constante> constantes = constante->CargarConstantes();
    vector<Constantes_Variables::Constante> variables = constante->CargarVariables();


};