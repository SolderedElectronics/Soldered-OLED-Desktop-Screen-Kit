#include "Helpers.h"

#pragma once

uint8_t digital_clock_state = 0;

void digitalClockCallback()
{
    digital_clock_state = (digital_clock_state + 1) % 2;
}

// Draw digital clock on the screen
void drawDigitalClock(OLED_Display &display,
                      SHTC3 &shtc,
                      PCF85063A &pcf85063a)
{
    // Reset text settings and clear the display
    resetText(display);
    display.clearDisplay();

    display.setFont(&DSEG14_Modern_Mini_Regular_15);
    display.setTextSize(2);
    display.setTextColor(WHITE);

    if (digital_clock_state == 0)
        display.setCursor(6, 40);
    else
        display.setCursor(6, 47);

    // Get current time
    int secs = pcf85063a.getSecond(),
        mins = pcf85063a.getMinute(),
        hours = pcf85063a.getHour();

    if (hours < 10)
        display.print("0");
    display.print(hours);

    if (millis() & 512)
        display.print(":");
    else
        display.setCursor(display.getCursorX() + 8, display.getCursorY());

    if (mins < 10)
        display.print("0");
    display.print(mins);

    shtc.sample();
    float temp, humidity;
    temp = shtc.readTempC();
    humidity = shtc.readHumidity();

    if (digital_clock_state == 0)
    {
        display.setFont();
        display.setTextSize(1);

        char a[40];
        sprintf(a, "%02d s  %02.2f C  %02.2f %%  ", secs, temp, humidity);

        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(a, 0, 0, &x1, &y1, &w, &h);

        int offset = (millis() / 200) % w;

        display.setCursor(-offset, 55);
        display.setTextWrap(false);

        display.print(a);
        display.print(a);
    }

    display.display();
}
