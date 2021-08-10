#include "OLED-Display-SOLDERED.h"

#pragma once

inline void drawPolarLine(OLED_Display &display, int x, int y, float phi, int from, int to)
{
    int x1 = x + (int)(cos(phi) * (float)from);
    int y1 = y + (int)(sin(phi) * (float)from);
    int x2 = x + (int)(cos(phi) * (float)to);
    int y2 = y + (int)(sin(phi) * (float)to);

    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
}

inline uint16_t isqrt(uint16_t n)
{
    uint16_t g = 0x8000;
    uint16_t c = 0x8000;
    for (;;)
    {
        if (1ll * g * g > n)
        {
            g ^= c;
        }
        c >>= 1;
        if (c == 0)
        {
            return g;
        }
        g |= c;
    }
    return 0;
}

uint8_t pattern[8] = {
    0b10001000,
    0b00000000,
    0b00100010,
    0b00000000,
    0b10001000,
    0b00000000,
    0b00100010,
    0b00000000,
};