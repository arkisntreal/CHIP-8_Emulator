#include <vector>

#ifndef CHIP8EMU_FONT_H_

namespace Font {
    //  Characters for the font are 4x5, so 5 unsigned chars are used
    //  and the 4 least significant bits are always 0
    typedef struct { unsigned char line[5]; } character ;

    void initializeFont(std::vector<character> &font);
}    

#endif // !CHIP8EMU_FONT_H_
#define CHIP8EMU_FONT_H_()
