#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Helpers.h"
#include "fonts/DSEG14_Modern_Mini_Regular_15.h"

#pragma once

void drawDigitalClock(OLED_Display &display,
                      BME280 &bme280,
                      PCF85063A &pcf85063a)
{
    resetText(display);
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

    char a[] = "00 s  00.00 C  00.00 %  0000.00 hPa  ";

    a[0] = secs / 10 % 10 + '0';
    if (a[0] == '0')
        a[0] = ' ';
    a[1] = secs % 10 + '0';

    a[6] = (int)temp / 10 % 10 + '0';
    if (a[6] == '0')
        a[6] = ' ';
    a[7] = (int)temp % 10 + '0';
    a[9] = (int)abs(temp * 10) % 10 + '0';
    a[10] = (int)abs(temp * 100) % 10 + '0';

    a[15] = (int)humidity / 10 % 10 + '0';
    if (a[15] == '0')
        a[15] = ' ';
    a[16] = (int)humidity % 10 + '0';
    a[18] = (int)abs(humidity * 10) % 10 + '0';
    a[19] = (int)abs(humidity * 100) % 10 + '0';

    a[24] = (int)pressure / 1000 % 10 + '0';
    if (a[24] == '0')
        a[24] = ' ';

    a[25] = (int)pressure / 100 % 10 + '0';
    a[26] = (int)pressure / 10 % 10 + '0';
    a[27] = (int)pressure % 10 + '0';

    a[29] = (int)(pressure * 10) % 10 + '0';
    a[30] = (int32_t)(pressure * 100) % 10 + '0';

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
