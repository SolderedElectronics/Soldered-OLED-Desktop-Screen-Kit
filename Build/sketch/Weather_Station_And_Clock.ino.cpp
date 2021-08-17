#include <Arduino.h>
#line 1 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
#undef SERIAL

#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Screen_Analog_Clock.h"
#include "Screen_Digital_Clock.h"
#include "Screen_World_Clock.h"
#include "Screen_Stopwatch.h"
#include "Screen_Timer.h"
#include "Screen_Menu.h"

#include "avdweb_Switch.h"
#include <FrequencyTimer2.h>

#include "Helpers.h"

OLED_Display display;
BME280 bme280;
PCF85063A pcf85063a;

uint8_t state = 4, lastState = 0;
Switch btn1(2);
Switch btn2(3);

#line 27 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void btn1CallbackSingle(void *s);
#line 33 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void btn1CallbackDouble(void *s);
#line 44 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void btn2CallbackPushed(void *s);
#line 50 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void btn2CallbackSingle(void *s);
#line 58 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void btn2CallbackDouble(void *s);
#line 68 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void btn2CallbackLongPress(void *s);
#line 74 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void timer2ISR();
#line 80 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void setup();
#line 104 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void loop();
#line 27 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Weather_Station_And_Clock.ino"
void btn1CallbackSingle(void *s)
{
    if (state != 5)
        state = (state + 1) % 5;
}

void btn1CallbackDouble(void *s)
{
    if (state == 5)
        state = lastState;
    else
    {
        lastState = state;
        state = 5;
    }
}

void btn2CallbackPushed(void *s)
{
    if (state == 4)
        stopwatchStateCallback();
}

void btn2CallbackSingle(void *s)
{
    if (state == 2)
        timerStateCallback();
    else if (state == 5)
        menuIncrement();
}

void btn2CallbackDouble(void *s)
{
    if (state == 2)
        timerSetupCallback();
    else if (state == 3)
        worldClockCallback();
    else if (state == 4)
        stopwatchSetupCallback();
}

void btn2CallbackLongPress(void *s)
{
    if (state == 5)
        menuStateCallback();
}

void timer2ISR()
{
    btn1.poll();
    btn2.poll();
}

void setup()
{
    pinMode(5, OUTPUT);

    // Serial.begin(115200);

    FrequencyTimer2::setPeriod(500);
    FrequencyTimer2::setOnOverflow(timer2ISR);

    btn1.setSingleClickCallback(&btn1CallbackSingle, (void *)"0");
    btn1.setDoubleClickCallback(&btn1CallbackDouble, (void *)"0");

    btn2.setSingleClickCallback(&btn2CallbackSingle, (void *)"0");
    btn2.setDoubleClickCallback(&btn2CallbackDouble, (void *)"0");
    btn2.setLongPressCallback(&btn2CallbackLongPress, (void *)"0");
    btn2.setPushedCallback(&btn2CallbackPushed, (void *)"0");

    bme280.begin();
    pcf85063a.begin();
    display.begin();

    delay(2000);
}

void loop()
{
    checkTimer(display, bme280, pcf85063a);

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
        drawTimer(display, bme280, pcf85063a, 0);
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
    case 5:
        drawMenu(display, bme280, pcf85063a);
        delay(10);
        break;
    default:
        break;
    }
}

