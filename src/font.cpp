#include <vector>

#include "font.h"

void Font::initializeFont(std::vector<Font::character> &font) {
    //  Numbers
    font.push_back(Font::character{0b01100000, 0b10010000, 0b10010000,
                                   0b10010000, 0b01100000});
    font.push_back(Font::character{0b01100000, 0b00100000, 0b00100000,
                                   0b00100000, 0b01110000});
    font.push_back(Font::character{0b01100000, 0b10010000, 0b00100000,
                                   0b01000000, 0b11110000});
    font.push_back(Font::character{0b11100000, 0b00010000, 0b01100000,
                                   0b00010000, 0b11100000});
    font.push_back(Font::character{0b10010000, 0b10010000, 0b01110000,
                                   0b00010000, 0b00010000});
    font.push_back(Font::character{0b11110000, 0b10000000, 0b11110000,
                                   0b00010000, 0b11110000});
    font.push_back(Font::character{0b11110000, 0b10000000, 0b11110000,
                                   0b10010000, 0b11110000});
    font.push_back(Font::character{0b11110000, 0b00010000, 0b00010000,
                                   0b00010000, 0b00010000});
    font.push_back(Font::character{0b11110000, 0b10010000, 0b11110000,
                                   0b10010000, 0b11110000});
    font.push_back(Font::character{0b11110000, 0b10010000, 0b11110000,
                                   0b00010000, 0b11110000});
    //  Letters
    font.push_back(Font::character{0b11110000, 0b10010000, 0b11110000,
                                   0b10010000, 0b10010000});
    font.push_back(Font::character{0b11100000, 0b10010000, 0b11100000,
                                   0b10010000, 0b11100000});
    font.push_back(Font::character{0b01110000, 0b10000000, 0b10000000,
                                   0b10000000, 0b01110000});
    font.push_back(Font::character{0b11100000, 0b10010000, 0b10010000,
                                   0b10010000, 0b11100000});
    font.push_back(Font::character{0b11110000, 0b10000000, 0b11100000,
                                   0b10000000, 0b11110000});
    font.push_back(Font::character{0b11110000, 0b10000000, 0b11100000,
                                   0b10000000, 0b10000000});
}
