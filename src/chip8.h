#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>

#include "font.h"
#include "window.h"

#ifndef CHIP8EMU_CHIP8_H_

class Chip8 {
    //  The CHIP-8 has 35 opcodes which are all two bytes long.
    unsigned short opcode;
    //  The CHIP-8 has 4K memory in total.
    unsigned char memory[4096];

    //  The CHIP-8 has 15 8-bit general purpose registers named V0, V1 up to VE.
    //  The 16th register is used for the 'carry flag'.
    unsigned char V[16];

    //  Index register and program counter. Value between 0x000 to 0xFFF
    unsigned short I;
    unsigned short pc;

    //  0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    //  0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    //  0x200-0xFFF - Program ROM and work RAM

    //  Interrupts and hardware registers. When set above zero
    //  they will count down to zero.
    unsigned char delay_timer;
    unsigned char sound_timer;

    //  Anytime you perform a jump or call a subroutine, store the
    //  program counter in the stack before proceeding. The system
    //  has 16 levels of stack and in order to remember which level
    //  of the stack is used, you need the stack pointer (sp)
    unsigned short stack[16];
    unsigned short sp;

    //  Current state of the key (0x0-0xF)
    unsigned char key[16];

    Window *window_ptr;

  public:
    //  This is the screen, CHIP-8 has a total of 2048 pixels,
    //  (64 x 32). The pixel state is always 1 or 0.
    unsigned char gfx[64][32];
    bool drawFlag;

    Chip8(Window &screen);

    void initialize();

    void loadGame(const char *game);

    void emulate();

    void setKey(SDL_Keysym kt, uint8_t ope);

    //  A 4x5 Font for the emulator to use
    std::vector<Font::character> font;
};

#endif // CHIP8EMU_CHIP8_H_
#define CHIP8EMU_CHIP8_H_()
