#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Screen_Analog_Clock.h"
#include "Screen_Digital_Clock.h"
#include "Screen_World_Clock.h"
#include "Screen_Stopwatch.h"
#include "Screen_Timer.h"

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
    drawTimer(display, bme280, pcf85063a);
    delay(1000);
    drawWorldClock(display, bme280, pcf85063a);
    delay(1000);
    drawAnalogClock(display, bme280, pcf85063a);
    delay(1000);
    drawDigitalClock(display, bme280, pcf85063a);
    delay(1000);
    drawStopwatch(display, bme280, pcf85063a);
    delay(1000);
}