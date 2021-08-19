#line 1 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Screen_Analog_Clock.h"
#include "SHTC3-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Helpers.h"

#pragma once

uint8_t analog_clock_state = 0;

void analogClockCallback()
{
    analog_clock_state = (analog_clock_state + 1) % 2;
}

void drawAnalogClock(OLED_Display &display,
                     SHTC3 &shtc,
                     PCF85063A &pcf85063a)
{
    resetText(display);
    display.clearDisplay();

    int secs = pcf85063a.getSecond(),
        mins = pcf85063a.getMinute(),
        hours = pcf85063a.getHour();

    int centerX, centerY;

    if (analog_clock_state == 0)
        centerX = 64 - 30, centerY = 32;
    else
        centerX = 64, centerY = 32;

    for (int i = 0; i < 24; ++i)
        drawPolarLine(display, centerX, centerY, i / 24.0 * TWO_PI, (i % 2 == 0 ? 24 : 28), 30);

    display.drawCircle(centerX, centerY, 2, SSD1306_WHITE);
    drawPolarLine(display, centerX, centerY, TWO_PI * mins / 60.0 - HALF_PI, 3, 30);
    drawPolarLine(display, centerX, centerY, TWO_PI * hours / 60.0 - HALF_PI, 3, 15);

    if (analog_clock_state == 0)
    {
        shtc.sample();
        float temp, humidity;
        temp = shtc.readTempC();
        humidity = shtc.readHumidity();

        display.setFont();
        display.setTextSize(1);
        display.setTextColor(WHITE);

        display.setCursor(74, 20);
        display.print(temp);
        display.print(" C");

        display.setCursor(74, 40);
        display.print(humidity);
        display.print(" %");
    }

    drawPolarLine(display, centerX, centerY + 10, TWO_PI * secs / 60.0 - HALF_PI, 0, 6);

    display.display();
}
