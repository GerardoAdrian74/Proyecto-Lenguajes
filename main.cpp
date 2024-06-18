#define CATCH_CONFIG_RUNNER
/*

#include <SDL.h>
#include <SDL_ttf.h>
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
}*/


#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "Constantes_Variables.hpp"
using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
class ComboBox {
public:
    ComboBox(int x, int y, int w, int h, const std::vector<std::string>& items, TTF_Font* font, SDL_Renderer* renderer)
        : rect{ x, y, w, h }, items(items), font(font), renderer(renderer), selectedItemIndex(-1), expanded(false) {
        bgColor = { 255, 255, 255, 255 }; // White
        textColor = { 0, 0, 0, 255 };     // Black
        outlineColor = { 0, 0, 0, 255 };  // Black
    }

    void handleEvent(SDL_Event& e) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            SDL_Point mousePoint = { x, y };
            if (SDL_PointInRect(&mousePoint, &rect)) {
                expanded = !expanded;
            }
            else if (expanded) {
                for (size_t i = 0; i < items.size(); ++i) {
                    SDL_Rect itemRect = { rect.x, rect.y + static_cast<int>(rect.h * (i + 1)), rect.w, rect.h };
                    if (SDL_PointInRect(&mousePoint, &itemRect)) {
                        selectedItemIndex = i;
                        expanded = false;
                        break;
                    }
                }
            }
            else {
                expanded = false;
            }
        }
    }
    void updateItems(const std::vector<std::string>& newItems) {
        items = newItems;
    }
    std::string getSelectedItem() const {
        if (selectedItemIndex >= 0 && selectedItemIndex < items.size()) {
            return items[selectedItemIndex];
        }
        return "";
    }
    void render() {
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
        SDL_RenderDrawRect(renderer, &rect);

        if (selectedItemIndex >= 0) {
            renderText(items[selectedItemIndex], rect.x + 5, rect.y + 5);
        }

        if (expanded) {
            for (size_t i = 0; i < items.size(); ++i) {
                SDL_Rect itemRect = { rect.x, rect.y + static_cast<int>(rect.h * (i + 1)), rect.w, rect.h };
                SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
                SDL_RenderFillRect(renderer, &itemRect);
                SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
                SDL_RenderDrawRect(renderer, &itemRect);
                renderText(items[i], itemRect.x + 5, itemRect.y + 5);
            }
        }
    }

private:
    SDL_Rect rect;
    std::vector<std::string> items;
    int selectedItemIndex;
    bool expanded;
    SDL_Color bgColor;
    SDL_Color textColor;
    SDL_Color outlineColor;
    TTF_Font* font;
    SDL_Renderer* renderer;

    void renderText(const std::string& text, int x, int y) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dstRect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
};
bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *window = SDL_CreateWindow("Evaluador de Expresiones", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void close(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color textColor) {
    if (text.empty()) {
        return; // Evita renderizar texto vac�o
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect renderQuad = { x - textWidth / 2, y - textHeight / 2, textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroyTexture(textTexture);
}

std::vector<std::string> loadConstants(vector<Constantes_Variables::Constante> arr) {
    std::vector<std::string> constants;
    for (Constantes_Variables::Constante tmp : arr) {
        string t,p;
        t = tmp.clave;
        t += '=';
        p=to_string(tmp.valor);
        if (p.find('.') != std::string::npos) {
            p.erase(p.find_last_not_of('0') + 1);
            if (p.back() == '.') {
                p.pop_back();
            }
        }
        t += p;
        cout << "Clave: " << t << endl;
        constants.push_back(t);
    }
    return constants;
}
std::vector<std::string> loadVariables(vector<Constantes_Variables::Constante> arr) {
    std::vector<std::string> variables;
    for (Constantes_Variables::Constante tmp : arr) {
        string t,p;
        t = tmp.clave;
        t += '=';
        p = to_string(tmp.valor);
        if (p.find('.') != std::string::npos) {
            p.erase(p.find_last_not_of('0') + 1);
            if (p.back() == '.') {
                p.pop_back();
            }
        }
        t += p;
        cout << "Clave: " << t << endl;
        variables.push_back(t);
    }
    return variables;
}
void drawUI(SDL_Renderer* renderer, TTF_Font* font, const std::string& expression, const std::vector<std::string>& history, vector<std::string>& variables, const std::vector<std::string>& constants, ComboBox& comboBox) {
    int contador = 0;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect constantesBox = { 50, 50, 200, 500 };
    SDL_Rect ingresoBox = { 300, 50, 400, 150 };
    SDL_Rect variablesBox = { 750, 50, 200, 500 };
    SDL_Rect historialBox = { 300, 250, 400, 300 };
    SDL_Rect botonBox = { ingresoBox.x + ingresoBox.w / 4, ingresoBox.y + ingresoBox.h + 1.5, ingresoBox.w / 2, 50 };
    SDL_Rect comboBoxButtonBox = { ingresoBox.x + ingresoBox.w / 4, 280, ingresoBox.w / 2, 50 }; //Boton combo
    SDL_RenderDrawRect(renderer, &constantesBox);
    SDL_RenderDrawRect(renderer, &ingresoBox);
    SDL_RenderDrawRect(renderer, &variablesBox);
    SDL_RenderDrawRect(renderer, &historialBox);
    SDL_RenderDrawRect(renderer, &botonBox);
    SDL_RenderDrawRect(renderer, &comboBoxButtonBox);

    SDL_Color textColor = { 0, 0, 0, 255 };

    renderText(renderer, font, "Constantes", constantesBox.x + constantesBox.w / 2, constantesBox.y + 20, textColor);
    renderText(renderer, font, "Ingreso de Expresion", ingresoBox.x + ingresoBox.w / 2, ingresoBox.y + 20, textColor);
    renderText(renderer, font, expression, ingresoBox.x + ingresoBox.w / 2, ingresoBox.y + ingresoBox.h / 2, textColor);
    renderText(renderer, font, "Variables", variablesBox.x + variablesBox.w / 2, variablesBox.y + 20, textColor);
    renderText(renderer, font, "Historial", historialBox.x + historialBox.w / 2, historialBox.y + 20, textColor);
    renderText(renderer, font, "Procesar Expresion", botonBox.x + botonBox.w / 2, botonBox.y + botonBox.h / 2, textColor);
    // Combo boton
    renderText(renderer, font, "Agregar al Evaluador", comboBoxButtonBox.x + comboBoxButtonBox.w / 2, comboBoxButtonBox.y + comboBoxButtonBox.h / 2, textColor); 

    int yOffset = constantesBox.y + 40;
    for (const std::string& constant : constants) {
        renderText(renderer, font, constant, constantesBox.x + constantesBox.w / 2, yOffset, textColor);
        yOffset += 30;
    }

    yOffset = variablesBox.y + 40;
    for (const std::string& variable : variables) {
        renderText(renderer, font, variable, variablesBox.x + variablesBox.w / 2, yOffset, textColor);
        yOffset += 30;
    }

    yOffset = historialBox.y + 40;
    /*
    for (const std::string& expr : history) {
        renderText(renderer, font, expr, historialBox.x + historialBox.w / 2, yOffset, textColor);
        yOffset += 30;
    }*/

    // **L�nea a�adida: Renderiza el ComboBox**
    comboBox.render();

    SDL_RenderPresent(renderer);
}

bool isMouseInsideBox(int mouseX, int mouseY, SDL_Rect box) {
    return (mouseX > box.x) && (mouseX < box.x + box.w) &&
        (mouseY > box.y) && (mouseY < box.y + box.h);
}

bool isConstant(const std::string& variable) {
    return variable == "PI" || variable == "E";
}

void processExpression(const std::string& expression, std::unordered_map<std::string, int>& variables) {
    size_t eqPos = expression.find('=');
    if (eqPos != std::string::npos) {
        std::string var = expression.substr(0, eqPos);
        std::string val = expression.substr(eqPos + 1);
        var.erase(remove_if(var.begin(), var.end(), isspace), var.end());
        val.erase(remove_if(val.begin(), val.end(), isspace), val.end());

        if (!isConstant(var)) {
            variables[var] = std::stoi(val);
        }
        else {
            std::cout << "Error: No se puede asignar a una constante." << std::endl;
        }
    }
}

int main(int argc, char* args[]) {
    Constantes_Variables obj;
    vector<Constantes_Variables::Constante> ListaConstantes;
    vector<Constantes_Variables::Constante> ListaVariables;

    ListaConstantes = obj.CargarConstantes();
    ListaVariables = obj.CargarVariables();
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(&window, &renderer)) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("LibreFranklin-Bold.ttf", 20);
    if (font == nullptr) {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        close(window, renderer, font);
        return -1;
    }

    std::vector<std::string> constants = loadConstants(ListaConstantes);
    std::vector<std::string> items = loadVariables(ListaVariables);
    std::vector<std::string> items2;
    items2.push_back("prueba");
    items2.push_back("hola");
    items2.push_back("test");
    ComboBox comboBox(350, 350, 300, 30, items2, font, renderer);
    bool quit = false;
    SDL_Event e;
    std::string expression;
    std::vector<std::string> history;
   
    SDL_Rect botonBox = { 300 + 400 / 4, 50 + 150 - 25, 400 / 2, 50 };
    //BotonCombobox
    SDL_Rect comboBoxButtonBox = { 300 + 400 / 4, 280, 400 / 2, 50 };
    SDL_StartTextInput();
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_TEXTINPUT) {
                expression += e.text.text;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && !expression.empty()) {
                    expression.pop_back();
                }
                else if (e.key.keysym.sym == SDLK_RETURN) {
                    std::cout << "Expresion ingresada: " << expression << std::endl;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseInsideBox(x, y, botonBox)) {
                    obj.agregarVariable(ListaConstantes, ListaVariables, expression);
                    ListaVariables = obj.CargarVariables();
                    items = loadVariables(ListaVariables);
                    comboBox.updateItems(items2);
                    expression.clear();
                }
                if (isMouseInsideBox(x, y, comboBoxButtonBox)) { // Check for ComboBox button press
                    std::string selectedItem = comboBox.getSelectedItem();
                    std::cout << "Selected item: " << selectedItem << std::endl;
                    expression = comboBox.getSelectedItem();
                }
            }
            comboBox.handleEvent(e);
        }
         ListaVariables = obj.CargarVariables();
        drawUI(renderer, font, expression, history, items, constants, comboBox);
    }
    SDL_StopTextInput();
    close(window, renderer, font);

    return 0;
}