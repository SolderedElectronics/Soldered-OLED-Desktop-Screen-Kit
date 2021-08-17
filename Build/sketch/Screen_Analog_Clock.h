#line 1 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Screen_Analog_Clock.h"
#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Helpers.h"

#pragma once

void drawAnalogClock(OLED_Display &display,
                     BME280 &bme280,
                     PCF85063A &pcf85063a)
{
    resetText(display);
    display.clearDisplay();

    for (int i = 0; i < 24; ++i)
        drawPolarLine(display, 64 - 30, 32, i / 24.0 * TWO_PI, (i % 2 == 0 ? 24 : 28), 30);

    int secs = pcf85063a.getSecond(),
        mins = pcf85063a.getMinute(),
        hours = pcf85063a.getHour();

    display.drawCircle(64 - 30, 32, 2, SSD1306_WHITE);
    drawPolarLine(display, 64 - 30, 32, TWO_PI * mins / 60.0 - HALF_PI, 3, 30);
    drawPolarLine(display, 64 - 30, 32, TWO_PI * hours / 60.0 - HALF_PI, 3, 15);

    float temp, humidity, pressure;
    bme280.readSensorData(temp, humidity, pressure);

    display.setFont();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(74, 10);
    display.print(temp);
    display.print(" C");

    display.setCursor(74, 30);
    display.print(humidity);
    display.print(" %");

    display.setCursor(74, 50);
    display.print((int)pressure);
    display.print(" hPa");

    drawPolarLine(display, 64 - 30, 32 + 10, TWO_PI * secs / 60.0 - HALF_PI, 0, 6);

    display.display();
}
