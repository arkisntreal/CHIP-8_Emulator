#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>

#include "src/chip8.h"
#include "src/window.h"

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 256;

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

    Window window("CHIP-8 Emulator", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    Chip8 emulator(window);

    emulator.initialize();
    emulator.loadGame("/home/arc/Projects/C++/CHIP-8_Emulator/Roms/pong.rom");

    double lifetime = 0.0;
    const double TIME_STEP = 1.0 / 6000.0;
    
    double accumulator = 0.0;
    double currentTime = SDL_GetTicks() * 0.001;

    SDL_Event e; 
    bool running = true;
    while (running) { 
        float newTime = SDL_GetTicks() * 0.001;
        float frameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= TIME_STEP) {
            emulator.emulate();

            if (emulator.drawFlag) {
                window.render(emulator.gfx);
                emulator.drawFlag = false;
            }

            while (SDL_PollEvent(&e) != 0) {
                switch (e.type) {
                    case SDL_QUIT: {
                        running = false;
                    break; }

                    case SDL_KEYDOWN: {
                        emulator.setKey(e.key.keysym, 1);
                    break; }

                    case SDL_KEYUP: {
                        emulator.setKey(e.key.keysym, 0);
                    break; }
                }
            }
            
            accumulator -= TIME_STEP;
            lifetime += TIME_STEP;
        }
    }

    window.destroy();
    SDL_Quit();

    return 0;
}

