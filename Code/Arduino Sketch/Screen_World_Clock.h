#include "SHTC3-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#pragma once

#include "World.h"

#include "Helpers.h"
#include "fonts/DSEG14_Modern_Mini_Regular_15.h"

int city_offset = 95;

char city0[] = "ZAG";
int city0_x, city0_y, city0_t;
char city1[] = "ZAG";
int city1_x, city1_y, city1_t;
char city2[] = "ZAG";
int city2_x, city2_y, city2_t;

void worldClockCallback()
{
    int num_shuffles = 50;

    int n = sizeof cityName / sizeof cityName[0];
    int idx[n];

    for (int i = 0; i < n; ++i)
        idx[i] = i;

    for (int j = 0; j < num_shuffles; j++)
    {
        for (int i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = idx[j];
            idx[j] = idx[i];
            idx[i] = t;
        }
    }

    strcpy(city0, cityName[idx[0]]);
    city0_x = cityCoordAndZone[3 * idx[0]];
    city0_y = cityCoordAndZone[3 * idx[0] + 1];
    city0_t = cityCoordAndZone[3 * idx[0] + 2];
    strcpy(city1, cityName[idx[1]]);
    city1_x = cityCoordAndZone[3 * idx[1]];
    city1_y = cityCoordAndZone[3 * idx[1] + 1];
    city1_t = cityCoordAndZone[3 * idx[1] + 2];
    strcpy(city2, cityName[idx[2]]);
    city2_x = cityCoordAndZone[3 * idx[2]];
    city2_y = cityCoordAndZone[3 * idx[2] + 1];
    city2_t = cityCoordAndZone[3 * idx[2] + 2];
}

void drawWorldClock(OLED_Display &display,
                    SHTC3 &shtc,
                    PCF85063A &pcf85063a)
{
    resetText(display);
    display.clearDisplay();
    // display.drawBitmap(0, 0, world_map, 200, 101, SSD1306_BLACK, SSD1306_WHITE);

    int secs = pcf85063a.getSecond(),
        mins = pcf85063a.getMinute(),
        hours = pcf85063a.getHour();

    display.drawCircle(32, 32, 35, SSD1306_WHITE);
    display.startWrite();

    int R = 35;
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

    int x = (city0_x + city_offset) & 127, y = city0_y, r, _x;

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

    display.print(city0);
    display.print(" ");

    int v = (hours - getTimeZone() + city0_t + 48) % 24;
    if (v < 10)
        display.print("0");
    display.print(v);

    display.print(":");

    if (mins < 10)
        display.print("0");
    display.print(mins);

    x = (city1_x + city_offset) & 127, y = city1_y;

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
    display.print(city1);
    display.print(" ");

    v = (hours - getTimeZone() + city1_t + 48) % 24;
    if (v < 10)
        display.print("0");
    display.print(v);

    display.print(":");

    if (mins < 10)
        display.print("0");
    display.print(mins);

    x = (city2_x + city_offset) & 127, y = city2_y;

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
    display.print(city2);
    display.print(" ");

    v = (hours - getTimeZone() + city2_t + 48) % 24;
    if (v < 10)
        display.print("0");
    display.print(v);

    display.print(":");

    if (mins < 10)
        display.print("0");
    display.print(mins);

    display.display();

    delay(30);
}
