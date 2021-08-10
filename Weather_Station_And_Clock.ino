#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Screen_Analog_Clock.h"
#include "Screen_Digital_Clock.h"
#include "World.h"

#include "Helpers.h"

OLED_Display display;
BME280 bme280;
PCF85063A pcf85063a;

void setup()
{
    Serial.begin(115200);

    bme280.begin();
    pcf85063a.begin();
    display.begin();

    delay(2000);
}

void loop()
{
    display.clearDisplay();

    for (int i = 0; i < 24; ++i)
        drawPolarLine(display, 100, 32, i / 24.0 * TWO_PI, (i % 2 == 0 ? 20 : 18), 22);

    drawPolarLine(display, 100, 32, (millis() / 1000) * TWO_PI / 60.0, 0, 20);

    int secs = pcf85063a.getSecond(),
        mins = pcf85063a.getMinute(),
        hours = pcf85063a.getHour();

    static uint32_t lastS = secs, lastMs = 0;

    if (lastS != secs)
        lastMs = millis(), lastS = secs;

    display.setFont(&DSEG14_Modern_Mini_Regular_15);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(3, 40);

    uint32_t t = (millis() - lastMs) / 10;

    if (t == t % 10)
        display.print("0");
    display.print(t);
    display.print(":");
    if (t == t % 10)
        display.print("0");
    display.print(t);

    display.setFont();
    display.setTextSize(1);
    display.setCursor(45, 50);

    if (t == t % 10)
        display.print("0");
    display.print(t);

    display.display();
}