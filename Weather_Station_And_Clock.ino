#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Screen_Analog_Clock.h"
#include "Screen_Digital_Clock.h"
#include "Screen_World_Clock.h"
#include "Screen_Stopwatch.h"
#include "Screen_Timer.h"

#include "avdweb_Switch.h"
#include <FrequencyTimer2.h>

#include "Helpers.h"

OLED_Display display;
BME280 bme280;
PCF85063A pcf85063a;

uint8_t state = 2;
Switch btn1(2);
Switch btn2(3);

void btn1CallbackSingle(void *s)
{
    state = (state + 1) % 5;
}

void btn2CallbackSingle(void *s)
{
    if (state == 2)
        timerStateCallback();
}

void btn2CallbackDouble(void *s)
{
    if (state == 2)
        timerSetupCallback();
}

void timer2ISR()
{
    btn1.poll();
    btn2.poll();
}

void setup()
{
    pinMode(13, OUTPUT);
    // Serial.begin(115200);

    FrequencyTimer2::setPeriod(500);
    FrequencyTimer2::setOnOverflow(timer2ISR);

    btn1.setSingleClickCallback(&btn1CallbackSingle, (void *)"0");

    btn2.setSingleClickCallback(&btn2CallbackSingle, (void *)"0");
    btn2.setDoubleClickCallback(&btn2CallbackDouble, (void *)"1");

    bme280.begin();
    pcf85063a.begin();
    display.begin();

    delay(2000);
}

void loop()
{
    switch (state)
    {
    case 0:
        drawAnalogClock(display, bme280, pcf85063a);
        delay(100);
        break;
    case 1:
        drawDigitalClock(display, bme280, pcf85063a);
        delay(100);
        break;
    case 2:
        drawTimer(display, bme280, pcf85063a);
        delay(10);
        break;
    case 3:
        drawWorldClock(display, bme280, pcf85063a);
        delay(10);
        break;
    case 4:
        drawStopwatch(display, bme280, pcf85063a);
        delay(10);
        break;
    default:
        break;
    }
}
