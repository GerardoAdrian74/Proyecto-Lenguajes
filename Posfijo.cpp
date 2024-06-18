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

// Función para verificar si un carácter es un operador
bool Posfijo::es_operador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// Función para validar la expresión infija
bool Posfijo::validar_expresion(const string& expresion) {
    stack<char> pila;
    bool lastWasOperator = true;  // True al inicio para manejar el primer número

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
            // Construimos el número si es dígito o punto decimal
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
                pila.pop(); // Eliminamos el '('
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

    // Vaciamos la pila restante
    while (!pila.empty()) {
        resultado.push_back(string(1, pila.top()));
        pila.pop();
    }

    // Convertimos el vector resultado a una cadena de texto
    stringstream rpn;
    for (const string& s : resultado) {
        rpn << s << " ";
    }

    return rpn.str();
}

// Función para evaluar una expresión postfija (RPN)
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
    if (!validar_expresion(expresion)) {
        cout << "La expresión infija es inválida." << endl;
        return 1;
    }

    try {
        string postfija = infija_a_postfija(expresion);
        cout << "Expresion postfija: " << postfija << endl;
        double resultado = evaluar_postfija(postfija);
        cout << "Resultado final: " << resultado << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what() << endl;
        return 1;
    }
}
/*
int main() {
    string expresion = "10+(1+2)*2^3%2";
    Posfijo* resolver = new Posfijo();
    resolver->evaluar(expresion);


    return 0;
}
*/