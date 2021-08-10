#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Helpers.h"
#include "fonts/DSEG14_Modern_Mini_Regular_15.h"

#pragma once

inline void drawDigitalClock(OLED_Display &display,
                             BME280 &bme280,
                             PCF85063A &pcf85063a)
{
    display.clearDisplay();

    display.setFont(&DSEG14_Modern_Mini_Regular_15);
    display.setTextSize(2);
    display.setTextColor(WHITE);

    display.setCursor(9, 40);

    int secs = pcf85063a.getSecond(),
        mins = pcf85063a.getMinute(),
        hours = pcf85063a.getHour();

    if (hours < 10)
        display.print("0");
    display.print(hours);

    display.print(":");

    if (mins < 10)
        display.print("0");
    display.print(mins);

    float temp, humidity, pressure;
    bme280.readSensorData(temp, humidity, pressure);

    display.setFont();
    display.setTextSize(1);

    char a[128];
    sprintf(a, "%2d s  %2d.%2d C  %2d.%2d %%  %3d.%2d hPa  ",
            secs,
            (int)temp,
            (int)abs(temp * 100) % 100,
            (int)humidity,
            (int)(humidity * 100) % 100,
            (int)pressure,
            (int32_t)(pressure * 100) % 100);

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(a, 0, 0, &x1, &y1, &w, &h);

    int offset = (millis() / 200) % w;

    display.setCursor(-offset, 55);
    display.setTextWrap(false);

    display.print(a);
    display.print(a);

    display.display();
}
