
#include "Posfijo.h"






Posfijo::Posfijo() {

}

int Posfijo::tipoOperadores(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/' || op == '%') {
        return 2;
    }
    if (op == '^') {
        return 3;
    }
    return 0;
}


bool Posfijo::es_operador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}


bool Posfijo::validar_expresion(const string& expresion) {
    stack<char> pila;
    bool lastWasOperator = true;

    for (char c : expresion) {
        if (isspace(c)) {
            continue;
        }
        else if (isdigit(c) || c == '.') {
            lastWasOperator = false;
        }
        else if (c == '(') {
            pila.push(c);
            lastWasOperator = true;
        }
        else if (c == ')') {
            if (pila.empty() || lastWasOperator) {
                return false;
            }
            pila.pop();
        }
        else if (es_operador(c)) {
            if (lastWasOperator) {
                return false;
            }
            lastWasOperator = true;
        }
        else {
            return false;
        }
    }

    return pila.empty() && !lastWasOperator;
}

string Posfijo::infija_a_postfija(const string& expresion) {
    stack<char> pila;
    vector<string> resultado;
    stringstream numero;

    for (char c : expresion) {
        if (isdigit(c) || c == '.') {
            numero << c;
        }
        else {
            if (numero.str().length() > 0) {

                resultado.push_back(numero.str());
                numero.str("");
                numero.clear();
            }

            if (c == '(') {
                pila.push(c);
            }
            else if (c == ')') {
                while (!pila.empty() && pila.top() != '(') {
                    resultado.push_back(string(1, pila.top()));
                    pila.pop();
                }
                pila.pop();
            }
            else if (es_operador(c)) {
                while (!pila.empty() && tipoOperadores(pila.top()) >= tipoOperadores(c)) {
                    resultado.push_back(string(1, pila.top()));
                    pila.pop();
                }
                pila.push(c);
            }
        }
    }

    if (numero.str().length() > 0) {
        resultado.push_back(numero.str());
    }


    while (!pila.empty()) {
        resultado.push_back(string(1, pila.top()));
        pila.pop();
    }


    stringstream rpn;
    for (const string& s : resultado) {
        rpn << s << " ";
    }

    return rpn.str();
}


double Posfijo::evaluar_postfija(const string& expresion) {
    stack<double> pila;
    stringstream ss(expresion);
    string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1) || token.find('.') != string::npos) {
            pila.push(stod(token));
        }
        else {
            double op2 = pila.top(); pila.pop();
            double op1 = pila.top(); pila.pop();
            double resultado = 0;

            if (token == "+") {
                resultado = op1 + op2;
            }
            else if (token == "-") {
                resultado = op1 - op2;
            }
            else if (token == "*") {
                resultado = op1 * op2;
            }
            else if (token == "/") {
                if (op2 == 0) {
                    throw runtime_error("Error: División por cero");
                }
                resultado = op1 / op2;
            }
            else if (token == "%") {
                if (op2 == 0) {
                    throw runtime_error("Error: Módulo por cero");
                }
                resultado = fmod(op1, op2);
            }
            else if (token == "^") {
                resultado = pow(op1, op2);
            }

            pila.push(resultado);
        }
    }

    return pila.top();
}

int Posfijo::evaluar(const string& expresion) {
    string nuevo = convertirVariable(variables, constantes, expresion);
    if (!validar_expresion(nuevo)) {
        cout << "La expresión infija es inválida." << endl;
        return 1;
    }

    try {
        string postfija = infija_a_postfija(nuevo);
        cout << "Expresion postfija: " << postfija << endl;
        double resultado = evaluar_postfija(postfija);
        cout << "Resultado final: " << resultado << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what() << endl;
        return 1;
    }
}

int Posfijo::esVariable(vector<Constantes_Variables::Constante>& variables, char expresion) {
    string var(1, expresion);
    for (const auto& v : variables) {
        if (v.clave == var) {
            return v.valor;
        }
    }
    return NULL;
}
int Posfijo::esConstante(vector<Constantes_Variables::Constante>& constantes, char expresion) {
    string constante(1, expresion);
    for (const auto& c : constantes) {
        if (c.clave == constante) {
            return c.valor;
        }
    }
    return NULL;
}
string Posfijo::convertirVariable(vector<Constantes_Variables::Constante>& variables, vector<Constantes_Variables::Constante>& constantes, string expresion) {
    string expresionConvertida;
    int var;
    int constante;
    for (char c : expresion) {
        if (!isalpha(c)) {
            expresionConvertida = expresionConvertida + c;
        }
        if (isalpha(c)) {
            var = esVariable(variables, c);
            constante = esConstante(constantes, c);
            if (var != NULL) {
                expresionConvertida = expresionConvertida + to_string(var);
            }
            else if (constante != NULL) {
                expresionConvertida = expresionConvertida + to_string(constante);
            }
            else {
                throw runtime_error("Variable no definida: " + c);

            }
        }
    }
    return expresionConvertida;
}
/*
int main() {

    string expresion = "y+(1+2)*2^3%5";
    cout << "Expresion " << expresion << endl;
    Posfijo* resolver = new Posfijo();

    resolver->evaluar(expresion);


    return 0;
}
*/