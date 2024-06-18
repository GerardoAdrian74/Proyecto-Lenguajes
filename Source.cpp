
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

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
        return; // Evita renderizar texto vacío
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

void drawUI(SDL_Renderer* renderer, TTF_Font* font, const std::string& expression, const std::vector<std::string>& history) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect constantesBox = { 50, 50, 200, 500 };
    SDL_Rect ingresoBox = { 300, 50, 400, 150 };
    SDL_Rect variablesBox = { 750, 50, 200, 500 };
    SDL_Rect historialBox = { 300, 250, 400, 300 };
    SDL_Rect botonBox = { ingresoBox.x + ingresoBox.w / 4, ingresoBox.y + ingresoBox.h + 1.5, ingresoBox.w / 2, 50 };
    SDL_RenderDrawRect(renderer, &constantesBox);
    SDL_RenderDrawRect(renderer, &ingresoBox);
    SDL_RenderDrawRect(renderer, &variablesBox);
    SDL_RenderDrawRect(renderer, &historialBox);
    SDL_RenderDrawRect(renderer, &botonBox);

    SDL_Color textColor = { 0, 0, 0, 255 };

    renderText(renderer, font, "Constantes", constantesBox.x + constantesBox.w / 2, constantesBox.y + 20, textColor);
    renderText(renderer, font, "Ingreso de Expresion", ingresoBox.x + ingresoBox.w / 2, ingresoBox.y + 20, textColor);
    renderText(renderer, font, expression, ingresoBox.x + ingresoBox.w / 2, ingresoBox.y + ingresoBox.h / 2, textColor);
    renderText(renderer, font, "Variables", variablesBox.x + variablesBox.w / 2, variablesBox.y + 20, textColor);
    renderText(renderer, font, "Historial", historialBox.x + historialBox.w / 2, historialBox.y + 20, textColor);
    renderText(renderer, font, "Procesar Expresion", botonBox.x + botonBox.w / 2, botonBox.y + botonBox.h / 2, textColor);

    int yOffset = historialBox.y + historialBox.h - 250;
    for (const std::string& expr : history) {
        renderText(renderer, font, expr, historialBox.x + historialBox.w / 2, yOffset, textColor);
        yOffset += 30; 
    }

    SDL_RenderPresent(renderer);
}

bool isMouseInsideBox(int mouseX, int mouseY, SDL_Rect box) {
    return (mouseX > box.x) && (mouseX < box.x + box.w) &&
        (mouseY > box.y) && (mouseY < box.y + box.h);
}

int main(int argc, char* args[]) {
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

    bool quit = false;
    SDL_Event e;
    std::string expression;
    std::vector<std::string> history;

    SDL_Rect botonBox = { 300 + 400 / 4, 50 + 150 - 25, 400 / 2, 50 };
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
                    history.push_back(expression);
                    std::cout << "Expresion guardada: " << expression << std::endl;
                    expression.clear(); // Limpiar la expresión después de guardarla
                }
            }
        }

        drawUI(renderer, font, expression, history);
    }
    SDL_StopTextInput();
    close(window, renderer, font);

    return 0;
}
