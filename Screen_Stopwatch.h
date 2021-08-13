#include "OLED-Display-SOLDERED.h"

#include "Helpers.h"
#include "fonts/DSEG14_Modern_Mini_Regular_15.h"

#pragma once

void drawStopwatch(OLED_Display &display,
                   BME280 &bme280,
                   PCF85063A &pcf85063a)
{
    resetText(display);
    display.clearDisplay();

    static uint32_t start = millis();

    uint32_t m = (millis() - start) / 1000 / 60 % 60;
    uint32_t s = (millis() - start) / 1000 % 60;
    uint32_t cs = (millis() - start) / 10 % 100;

    for (int i = 0; i < 24; ++i)
        drawPolarLine(display, 100, 32, i / 24.0 * TWO_PI, (i % 2 == 0 ? 20 : 18), 22);

    drawPolarLine(display, 100, 32, s * TWO_PI / 60.0 - HALF_PI, 0, 20);

    display.setFont(&DSEG14_Modern_Mini_Regular_15);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(3, 40);

    if (m == m % 10)
        display.print("0");
    display.print(m);
    display.print(":");
    if (s == s % 10)
        display.print("0");
    display.print(s);

    display.setFont();
    display.setTextSize(1);
    display.setCursor(45, 50);

    if (cs == cs % 10)
        display.print("0");
    display.print(cs);

    display.display();
}