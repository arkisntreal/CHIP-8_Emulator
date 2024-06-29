#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "window.h"

Window::Window(const char *title, int width, int height)
    : window(NULL), renderer(NULL) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_SHOWN);

    if (window == NULL)
        printf("Window failed to initialize! SDL_Error: %s\n", SDL_GetError());
    else {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
}

void Window::render(unsigned char windowData[64][32]) {
    Window::clear();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            if (windowData[j][i] == 1) {
                SDL_Rect pixel = {.x = j * 8, .y = i * 8, .w = 8, .h = 8};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
