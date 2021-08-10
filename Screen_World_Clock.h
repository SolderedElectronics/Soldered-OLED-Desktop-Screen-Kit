#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Helpers.h"
#include "fonts/DSEG14_Modern_Mini_Regular_15.h"

#pragma once

inline void drawWorldClock(OLED_Display &display,
                           BME280 &bme280,
                           PCF85063A &pcf85063a)
{
    display.clearDisplay();
    // display.drawBitmap(0, 0, world_map, 200, 101, SSD1306_BLACK, SSD1306_WHITE);

    display.drawCircle(32, 32, 35, SSD1306_WHITE);
    display.startWrite();

    int R = 35;

    int d0 = 10000, x0, y0,
        d1 = 10000, x1, y1,
        d2 = 10000, x2, y2;

    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 128; ++j)
        {
            if ((i - 32) * (i - 32) + (j - 32) * (j - 32) > R * R)
                continue;

            int _j = j + 64 - 32;
            if (!getWorldXY((_j + (millis() >> 7)) & 127, i))
                display.writePixel(j, i, SSD1306_WHITE);
            else if (!getWorldXY((-_j + 192 + (millis() >> 7)) & 127, i) &&
                     (pattern[i & 7] & (1 << (7 - (j & 7)))))
                display.writePixel(j, i, SSD1306_WHITE);
            else
                display.writePixel(j, i, SSD1306_BLACK);
        }

    display.endWrite();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    int x = 50, y = 20, r, _x;

    r = 35 - 0.03 * (y - 32) * (y - 32);
    _x = (x - (millis() >> 7) & 127);
    if (_x >= 64)
        _x = 127 - _x;
    _x -= 32;
    _x = _x * r / 32;
    _x += 32;

    display.drawLine(72, 12, _x + 1, y, SSD1306_WHITE);
    display.drawLine(71, 13, _x, y + 1, SSD1306_BLACK);

    display.setCursor(73, 10);
    display.print("ZAG");
    display.print(" 13:00");

    x = 100, y = 40;

    r = 35 - 0.03 * (y - 32) * (y - 32);
    _x = (x - (millis() >> 7) & 127);
    if (_x >= 64)
        _x = 127 - _x;
    _x -= 32;
    _x = _x * r / 32;
    _x += 32;

    display.drawLine(72, 32, _x + 1, y, SSD1306_WHITE);
    display.drawLine(71, 33, _x, y + 1, SSD1306_BLACK);

    display.setCursor(73, 30);
    display.print("NYC");
    display.print(" 13:00");

    x = 10, y = 50;

    r = 35 - 0.03 * (y - 32) * (y - 32);
    _x = (x - (millis() >> 7) & 127);
    if (_x >= 64)
        _x = 127 - _x;
    _x -= 32;
    _x = _x * r / 32;
    _x += 32;

    display.drawLine(72, 52, _x + 1, y, SSD1306_WHITE);
    display.drawLine(71, 53, _x, y + 1, SSD1306_BLACK);

    display.setCursor(73, 50);
    display.print("TKY");
    display.print(" 13:00");

    display.display();

    delay(30);
}
