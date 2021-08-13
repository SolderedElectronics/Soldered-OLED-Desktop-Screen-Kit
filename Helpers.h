#include "OLED-Display-SOLDERED.h"

#pragma once

void drawPolarLine(OLED_Display &display, int x, int y, float phi, int from, int to, float a = 1.0f, float b = 1.0f)
{
    int x1 = x + (int)(cos(phi) * a * (float)from);
    int y1 = y + (int)(sin(phi) * b * (float)from);
    int x2 = x + (int)(cos(phi) * a * (float)to);
    int y2 = y + (int)(sin(phi) * b * (float)to);

    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
}

uint16_t isqrt(uint16_t n)
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

void resetText(OLED_Display &display)
{
    display.setFont();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(true);
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