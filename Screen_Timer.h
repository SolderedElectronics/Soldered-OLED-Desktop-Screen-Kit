#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "World.h"

#include "Helpers.h"
#include "fonts/DSEG14_Modern_Mini_Regular_15.h"

#pragma once

inline void drawTimer(OLED_Display &display,
                      BME280 &bme280,
                      PCF85063A &pcf85063a)
{
    static uint32_t time = 100;

    resetText(display);

    display.clearDisplay();
    for (int i = 0; i < 32; ++i)
    {
        float val = tanh(40 * (i / 32.0 - 0.6)) + 1;
        drawPolarLine(display, 64, 32, TWO_PI * i / 32.0, 26, 28 + val * 2, 1.5, 1.0);
    }

    display.setFont(&DSEG14_Modern_Mini_Regular_15);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(64 - 26, 32 + 7);

    int m = (time - millis() / 1000) / 60 % 100,
        s = (time - millis() / 1000) % 60;

    if (m == m % 10)
        display.print("0");
    display.print(m);
    display.print(":");
    if (s == s % 10)
        display.print("0");
    display.print(s);
    display.display();
    delay(1000);
}
