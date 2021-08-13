#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "World.h"

#include "Helpers.h"
#include "fonts/DSEG14_Modern_Mini_Regular_15.h"

#pragma once

uint8_t timer_state = 0;
uint32_t start = 0;

uint8_t s;

uint8_t timer_set_m = 0, timer_set_s = 5;
uint32_t timeTotal, timer_stop;

void timerStateCallback()
{
    if (timer_state == 0 || timer_state == 1 || timer_state == 2)
        timer_state = (timer_state + 1) % 3;
    else if (timer_state == 3)
        timer_set_m = (timer_set_m + 5) % 100;
    else if (timer_state == 4)
        timer_set_s = (timer_set_s + 5) % 60;

    if (timer_state == 1)
    {
        start = millis();
        timeTotal = timer_set_m * 60 + timer_set_s - 1;
    }
    else if (timer_state == 2)
    {
        timer_stop = (timeTotal - (millis() - start) / 1000);
    }
}

void timerSetupCallback()
{
    if (timer_state == 0 || timer_state == 2)
        timer_state = 3;
    else if (timer_state == 3)
        timer_state = 4;
    else if (timer_state == 4)
        timer_state = 0;
}

void drawTimer(OLED_Display &display,
               BME280 &bme280,
               PCF85063A &pcf85063a)
{
    resetText(display);
    display.clearDisplay();

    display.setFont(&DSEG14_Modern_Mini_Regular_15);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(64 - 26, 32 + 7);

    for (int i = 0; i < 32; ++i)
    {
        float val;
        if (timer_state == 1)
            tanh(40 * (i / 32.0 - (1.0f * ((millis() - start))) / (1000 * timeTotal))) + 1;
        else
            val = 0;
        drawPolarLine(display, 64, 32, TWO_PI * i / 32.0, 26, 28 + val * 2, 1.5, 1.0);
    }

    switch (timer_state)
    {
    case 0:
    {
        if (timer_set_m == timer_set_m % 10)
            display.print("0");
        display.print(timer_set_m);
        display.print(":");
        if (timer_set_s == timer_set_s % 10)
            display.print("0");
        display.print(timer_set_s);
        display.display();
    }
    break;

    case 1:
    {
        int m = (timeTotal - (millis() - start) / 1000) / 60 % 100,
            s = (timeTotal - (millis() - start) / 1000) % 60;

        if ((millis() - start) / 1000 > timeTotal)
        {
            m = 0, s = 0;

            while (digitalRead(3) != LOW)
            {
                if (millis() & 512)
                {
                    digitalWrite(13, HIGH);
                    delay(2);
                    digitalWrite(13, LOW);
                    delay(2);
                }
            }

            timer_state = 2;
        }

        if (m == m % 10)
            display.print("0");
        display.print(m);
        display.print(":");
        if (s == s % 10)
            display.print("0");
        display.print(s);
        display.display();
    }
    break;

    case 2:
    {
        int m = timer_stop / 60 % 100,
            s = timer_stop % 60;

        if (m == m % 10)
            display.print("0");
        display.print(m);
        display.print(":");
        if (s == s % 10)
            display.print("0");
        display.print(s);
        display.display();
    }
    break;

    case 3:
    {
        if (millis() & 512)
        {
            if (timer_set_m == timer_set_m % 10)
                display.print("0");
            display.print(timer_set_m);
        }
        else
        {
            display.setCursor(display.getCursorX() + 26, display.getCursorY());
        }
        display.print(":");
        if (timer_set_s == timer_set_s % 10)
            display.print("0");
        display.print(timer_set_s);
        display.display();
    }
    break;

    case 4:
    {
        if (timer_set_m == timer_set_m % 10)
            display.print("0");
        display.print(timer_set_m);
        display.print(":");

        if (millis() & 512)
        {
            if (timer_set_s == timer_set_s % 10)
                display.print("0");
            display.print(timer_set_s);
        }
        else
        {
            display.setCursor(display.getCursorX() + 26, display.getCursorY());
        }
        display.display();
    }
    break;

    default:
        break;
    }
}
