#line 1 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Screen_Stopwatch.h"
#include "OLED-Display-SOLDERED.h"

#include "Helpers.h"
#include "fonts/DSEG14_Modern_Mini_Regular_15.h"

#pragma once

uint8_t stopwatch_state = 0;
uint32_t stopwatch_start = 0;
uint32_t stopwatch_extra = 0;

void stopwatchStateCallback()
{
    if (stopwatch_state == 0)
    {
        stopwatch_state = 1;

        stopwatch_start = millis();
        stopwatch_extra = 0;
    }
    else if (stopwatch_state == 1)
    {
        stopwatch_state = 2;

        stopwatch_extra += millis() - stopwatch_start;
    }
    else if (stopwatch_state == 2)
    {
        stopwatch_state = 1;

        stopwatch_start = millis();
    }
}

void stopwatchSetupCallback()
{
    stopwatch_state = 0;
}

void drawStopwatch(OLED_Display &display,
                   BME280 &bme280,
                   PCF85063A &pcf85063a)
{
    resetText(display);
    display.clearDisplay();

    uint32_t m, s, cs;

    if (stopwatch_state == 0)
        m = s = cs = 0;
    else if (stopwatch_state == 1)
    {
        m = (millis() - stopwatch_start + stopwatch_extra) / 1000 / 60 % 60;
        s = (millis() - stopwatch_start + stopwatch_extra) / 1000 % 60;
        cs = (millis() - stopwatch_start + stopwatch_extra) / 10 % 100;
    }
    else if (stopwatch_state == 2)
    {
        m = stopwatch_extra / 1000 / 60 % 60;
        s = stopwatch_extra / 1000 % 60;
        cs = stopwatch_extra / 10 % 100;
    }

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