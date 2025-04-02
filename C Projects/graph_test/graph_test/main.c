#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

void DrawGraph(SDL_Renderer *renderer) {
    int x;
    double y;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Белый цвет
    for (x = 0; x < WIDTH; x++) {
        y = HEIGHT / 2 – (int)(100 * sin(x * 2 * M_PI / WIDTH));
        SDL_RenderDrawPoint(renderer, x, (int)y);
    }
}

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }

    window = SDL_CreateWindow("Graph of sin(x)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Черный фон
    SDL_RenderClear(renderer);

    DrawGraph(renderer);

    SDL_RenderPresent(renderer);

    SDL_Delay(5000); // Задержка на 5 секунд

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
