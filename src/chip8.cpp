#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

#include "chip8.h"
#include "font.h"

Chip8::Chip8(Window &screen) : drawFlag(false), window_ptr(&screen) {}

void Chip8::initialize() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    window_ptr->clear();

    for (int i = 0; i < 16; i++) {
        stack[i] = 0;
    }
    for (int i = 0; i < 4096; i++) {
        memory[i] = 0;
    }
    for (int i = 0; i < 16; i++) {
        V[i] = 0;
    }
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            gfx[j][i] = 0;
        }
    }
    //  Loading the font
    Font::initializeFont(font); 
    for (int charNr = 0; charNr < font.size(); charNr++) {
        for (int lineNr = 0; lineNr < 5; lineNr++) {
            memory[charNr * 5 + lineNr] = font[charNr].line[lineNr];
        }
    }

    delay_timer = 0;
    sound_timer = 0;
}

void Chip8::loadGame(const char *game) {
    std::ifstream input(game, std::ios::binary);

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    printf("FILE:\n");
    for (int i = 0; i < buffer.size(); i++) {
        memory[i + 0x200] = buffer[i];
        printf("%i", memory[i + 0x200]);
    }
    printf("\n");
}

void Chip8::emulate() {
    //  Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    //  Decode and Execute Opcode
    printf("OPCODE: %04x:\n+-+ ", opcode);
    switch (opcode & 0xF000) {
        case 0x0000: {
            switch (opcode & 0x00FF) {
                case 0x00E0: {
                    printf("Clearing screen\n");
                    window_ptr->clear();
                    pc += 2;
                break; }

                case 0x00EE: {
                    //  Returns from a subroutine
                    printf("Returning from subroutine\n");
                    pc = stack[--sp] + 2;
                break; }
            }
        break; }

        case 0x1000: {
            //  Jump to address NNN (0x1NNN)
            printf("Jumping to %i\n", opcode & 0x0FFF);
            pc = opcode & 0x0FFF;
        break; }

        case 0x2000: {
            printf("Calling subroutine at %03x\n", opcode & 0x0FFF);
            stack[sp++] = pc;
            pc = opcode & 0x0FFF;
        break; }

        case 0x3000: {
            //  Skip next instuction if VX equals NN (0x3XNN)
            printf("Skip next instruction if VX == NN\n");
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4;
            else 
                pc += 2;
        break; }

        case 0x4000: {
            //  Skip next instruction if VX does not equal NN (0x4XNN)
            printf("Skip next instruction if VX != NN\n");
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
        break; }

        case 0x5000: {
            //  Skip next instruction if VX equals VY (0x5XY0)
            printf("Skip next instruction if VX == VY\n");
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
        break; }

        case 0x6000: {
            //  Setting VX to NN (0x6XNN)
            printf("Setting V%01x to %i\n", (opcode & 0x0F00) >> 8, (opcode & 0x00FF));
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
        break; }

        case 0x7000: {
            //  Add NN to VX (no change to carry flag) (0x7XNN)
            printf("Adding %02x to V%01x\n", opcode & 0x00FF, (opcode & 0x0F00) >> 8);
            V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            pc += 2;
        break; }

        case 0x8000: { // (0x8XYn)
            switch (opcode & 0x000F) {
                case 0x0000: {
                    //  Sets VX to the value of VY
                    printf("Set VX to VY\n");
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break; }

                case 0x0001: {
                    //  Sets VX to VX or VY (bitwise OR)
                    printf("Set VX to VX OR VY\n");
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break; }

                case 0x0002: {
                    //  Sets VX to VX and VY (bitwise AND)
                    printf("Set VX to VX AND VY\n");
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break; }

                case 0x0003: {
                    //  Sets VX to VX xor VY (bitwise XOR)
                    printf("Set VX to VX XOR VY\n");
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break; }

                case 0x0004: {
                    //  Adds VY to VX. VF is set to 1 when there is overflow, and 0 when not
                    printf("Added VY to VX\n");
                    if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 255)
                        V[0xF] = 1;
                    else 
                        V[0xF] = 0;
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break; }

                case 0x0005: {
                    //  VY is subtracted from VX. VF is set to 0 when there's an underflow, and 1 when not
                    printf("Subtracted VY from VX\n");
                    V[0xF] = V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break; }

                case 0x0006: {
                    //  Shifts VX to the right by 1, then stores the least significant bit of VX
                    //  prior to the shift into VF
                    printf("Shifted VX to the right by 1\n");
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x0001;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
                    pc += 2;
                break; }

                case 0x0007: {
                    //  Sets VX to VY minus VX. VF is set to 0 when there's an underflow, and 1 when not
                    printf("Set VX to VY minus VX\n");
                    V[0xF] = V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break; }

                case 0x000E: {
                    //  Shifts VX to the left by 1, then sets VF to 1 if the most significant bit
                    //  of VX prior to that shift was set, or to 0 if it was unset
                    printf("Shifted VX to the left by 1\n");
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x8000;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
                    pc += 2;
                break; }
            }
        break; }

        case 0x9000: {
            //  Skips the next instruction if VX does not equal VY. (0x9XY0)
            printf("Skip next instruction if VX != VY\n");
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else 
                pc += 2;
        break; }

        case 0xA000: {
            printf("Set I to %03x (%i)\n", opcode & 0x0FFF, opcode & 0x0FFF);
            I = opcode & 0x0FFF;
            pc += 2;
        break; }

        case 0xB000: {
            //  Jumps to the address NNN plus V0 (0xBNNN)
            printf("Jump to NNN plus V0\n");
            pc = V[0x0] + (opcode & 0x0FFF);
        break; }

        case 0xC000: {
            //  Sets VX to the result of a bitwise and operation on a random number (typically 0 to 255) and NN (0xCXNN)
            V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
            pc += 2;
        break; }

        case 0xD000: {
            printf("Drawing a sprite\n");
            int x = V[(opcode & 0x0F00) >> 8];
            int y = V[(opcode & 0x00F0) >> 4];
            V[0xF] = 0;
            for (int i = 0; i < (opcode & 0x000F); i++) {
                int offset = 7;
                for (int j = 0; j < 8; j++, offset--) {
                    if (gfx[x + j][y + i] == 1) 
                        V[0xF] = 1;
                    gfx[x + j][y + i] ^= (memory[I + i] >> offset) & 0b00000001;
                }
                printf("GFX[x]: %04x\nI: %i\n", memory[I + i], I + i);
            }
            drawFlag = true;
            pc += 2;
        break; }

        case 0xE000: {
            switch (opcode & 0x000F) {
                case 0x000E: {
                    //  Skips the next instruction if the key stored in VX is pressed (0xEX9E)
                    printf("Processing event, skip if pressed\n");
                    printf("V%01x: %i", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8]);
                    // SDL_Event e;
                    // while (SDL_PollEvent(&e) != 0) {
                    //     if (e.type == V[(opcode & 0x0F00) >> 8]) {
                    //         pc += 2;
                    //     }     
                    // }
                    pc += 2;
                break; }

                case 0x0001: {
                    //  Skips the next instruction if the key stored in VX is not pressed (0xEXA1)
                    printf("Processing event, skip if not pressed\n");
                    printf("V%01x: %i\n", (opcode & 0x0F00) >> 8, V[(opcode & 0x0F00) >> 8]);
                    if (key[V[(opcode & 0x0F00) >> 8]] == 1) {
                        printf("KEY IS PRESSED\n");
                        pc += 2;
                    } else {
                        printf("KEY IS NOT PRESSED\n");
                        pc += 4;
                    }
                    // SDL_Event e;
                    // int dangnabbit = 4;
                    // while (SDL_PollEvent(&e) != 0) {
                    //     if (e.type == V[(opcode & 0x0F00) >> 8]) {
                    //         dangnabbit -= 2;
                    //     }     
                    // }
                    // pc += dangnabbit;
                break; }
            }
        break; }

        case 0xF000: { // (0xFXnn)
            switch (opcode & 0x00FF) {
                case 0x0007: {
                    //  Sets VX to the value of the delay timer 
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                break; }

                case 0x000A: {
                    //  A key press is awaited, and then stored in VX (blocking operation,
                    //  all instruction halted until next key event) 
                    // V[(opcode & 0x0F00) >> 8] = awaitKeyPress();
                    SDL_Event e;
                    bool received = false;
                    while (!received) {
                        while (SDL_PollEvent(&e) != 0) {
                            V[(opcode & 0x0F00) >> 8] = e.type;
                        }
                    }
                    pc += 2;
                break; }

                case 0x0015: {
                    //  Sets the delay timer to VX
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break; }

                case 0x0018: {
                    //  Sets the sound timer to VX
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break; }

                case 0x001E: {
                    //  Adds VX to I. VF is not affected.
                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break; }

                case 0x0029: {
                    //  Sets I to the location of the sprite for the character in VX.
                    //  Characters 0-F are represented by a 4x5 font
                    printf("Set I to location of sprite for character in V%01x\n", (opcode & 0x0F00) >> 8);
                    I = V[(opcode & 0x0F00) >> 8] * 5;
                    pc += 2;
                break; }

                case 0x0033: {
                    //  Stores the binary-coded decimal representation of VX,
                    //  with the hundreds digit in memory at location in I,
                    //  the tens digit at location I+1, and the ones digit at location I+2
                    printf("Stored binary-coded decimal representation of VX\n");
                    int value = V[(opcode & 0x0F00) >> 8];
                    memory[I] = value / 100;
                    memory[I + 1] = (value / 10 ) % 10;
                    memory[I + 2] = value % 10;
                    pc += 2;
                break; }

                case 0x0055: {
                    //  Stores from V0 to VX (including VX) in memory, starting at address I.
                    //  The offset from I is increased by 1 for each value written, but I itself is left unmodified.
                    printf("Filling memory with values from V\n");
                    int j = 0;
                    for (int i = I; i <= ((opcode & 0x0F00) >> 8) + I; i++) {
                        memory[i] = V[j++];
                    }
                    pc += 2;
                break; }

                case 0x0065: {
                    //  Fills from V0 to VX (including VX) with values from memory, starting at address I.
                    //  The offset from I is increased by 1 for each value read, but I itself is left unmodified.
                    printf("Filling V with values from memory\n");
                    int j = 0;
                    for (int i = I; i <= ((opcode & 0x0F00) >> 8) + I; i++) {
                        V[j++] = memory[i];
                    }
                    pc += 2;
                break; }
            }
        break; }
    }

    //  Update timers
    if (delay_timer > 0)
        delay_timer--;

    if (sound_timer > 0) {
        if (sound_timer == 1) {}
            printf("BEEP!\n");
        sound_timer--;
    }
}

void Chip8::setKey(SDL_Keysym kt, uint8_t ope) {
    if (kt.sym == SDLK_1)
        key[0x1] = ope;
    else if (kt.sym == SDLK_2)
        key[0x2] = ope;
    else if (kt.sym == SDLK_3)
        key[0x3] = ope;
    else if (kt.sym == SDLK_4)
        key[0xC] = ope;
    else if (kt.sym == SDLK_q)
        key[0x4] = ope;
    else if (kt.sym == SDLK_w)
        key[0x5] = ope;
    else if (kt.sym == SDLK_e)
        key[0x6] = ope;
    else if (kt.sym == SDLK_r)
        key[0xD] = ope;
    else if (kt.sym == SDLK_a)
        key[0x7] = ope;
    else if (kt.sym == SDLK_s)
        key[0x8] = ope;
    else if (kt.sym == SDLK_d)
        key[0x9] = ope;
    else if (kt.sym == SDLK_f)
        key[0xE] = ope;
    else if (kt.sym == SDLK_z)
        key[0xA] = ope;
    else if (kt.sym == SDLK_x)
        key[0x0] = ope;
    else if (kt.sym == SDLK_c)
        key[0xB] = ope;
    else if (kt.sym == SDLK_v)
        key[0xF] = ope;
}

