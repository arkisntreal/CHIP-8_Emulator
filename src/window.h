#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#ifndef CHIP8EMU_WINDOW_H_

class Window {
    SDL_Window *window;
    SDL_Renderer *renderer;

  public:
    Window(const char *title, int width, int height);

    void destroy() { SDL_DestroyWindow(window); }
    void clear() { SDL_RenderClear(renderer); }
    void render(unsigned char windowData[64][32]);
};

#endif //  CHIP8EMU_WINDOW_H_
#define CHIP8EMU_WINDOW_H_()
