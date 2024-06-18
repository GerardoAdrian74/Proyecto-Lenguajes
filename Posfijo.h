#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>

using namespace std;
class Posfijo
{
public:
    Posfijo();
    int tipoOperadores(char op);
    bool es_operador(char c);
    bool validar_expresion(const string& expresion);
    string infija_a_postfija(const string& expresion);
    double evaluar_postfija(const string& expresion);
    int evaluar(const string& expresion);
private:


};
