#include "SHTC3-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Screen_Analog_Clock.h"
#include "Screen_Digital_Clock.h"
#include "Screen_World_Clock.h"
#include "Screen_Stopwatch.h"
#include "Screen_Timer.h"
#include "Screen_Menu.h"

#include "avdweb_Switch.h"
#include "ESP8266TimerInterrupt.h"

#include "Helpers.h"

OLED_Display display;
SHTC3 shtc;
PCF85063A pcf85063a;

ESP8266Timer ITimer;

uint8_t state = 1, lastState = 0;
Switch btn1(12);
Switch btn2(13);

bool btn1Single, btn1Double, btn2Pushed, btn2Single, btn2Double;

void btn1CallbackSingle(void *s)
{
    btn1Single = 1;
}

void btn1CallbackDouble(void *s)
{
    btn1Double = 1;
}

void btn2CallbackPushed(void *s)
{
    btn2Pushed = 1;
}

void btn2CallbackSingle(void *s)
{
    btn2Single = 1;
}

void btn2CallbackDouble(void *s)
{
    btn2Double = 1;
}

void timer2ISR()
{
    btn1.poll();
    btn2.poll();
}

void setup()
{
    worldClockCallback();
    pinMode(14, OUTPUT);

    Serial.begin(74880);

    ITimer.attachInterruptInterval(1000, timer2ISR);

    btn1.setSingleClickCallback(&btn1CallbackSingle, (void *)"0");
    btn1.setDoubleClickCallback(&btn1CallbackDouble, (void *)"0");

    btn2.setSingleClickCallback(&btn2CallbackSingle, (void *)"0");
    btn2.setDoubleClickCallback(&btn2CallbackDouble, (void *)"0");
    btn2.setPushedCallback(&btn2CallbackPushed, (void *)"0");

    shtc.begin();
    pcf85063a.begin();
    display.begin();

    EEPROM.begin(16);

    delay(2000);
}

void loop()
{
    if (btn1Single)
    {
        if (state != 5)
            state = (state + 1) % 5;
        btn1Single = 0;
    }
    if (btn1Double)
    {
        if (state == 5)
            state = lastState;
        else
        {
            lastState = state;
            state = 5;
        }
        btn1Double = 0;
    }
    if (btn2Pushed)
    {
        if (state == 4)
            stopwatchStateCallback();
        btn2Pushed = 0;
    }
    if (btn2Single)
    {
        if (state == 0)
            analogClockCallback();
        else if (state == 1)
            digitalClockCallback();
        else if (state == 2)
            timerStateCallback();
        else if (state == 5)
            menuIncrement();

        btn2Single = 0;
    }
    if (btn2Double)
    {
        if (state == 2)
            timerSetupCallback();
        else if (state == 3)
            worldClockCallback();
        else if (state == 4)
            stopwatchSetupCallback();
        else if (state == 5)
            menuStateCallback();
        btn2Double = 0;
    }

    checkTimer(display, shtc, pcf85063a);

    switch (state)
    {
    case 0:
        drawAnalogClock(display, shtc, pcf85063a);
        delay(100);
        break;
    case 1:
        drawDigitalClock(display, shtc, pcf85063a);
        delay(100);
        break;
    case 2:
        drawTimer(display, shtc, pcf85063a, 0);
        delay(10);
        break;
    case 3:
        drawWorldClock(display, shtc, pcf85063a);
        delay(10);
        break;
    case 4:
        drawStopwatch(display, shtc, pcf85063a);
        delay(10);
        break;
    case 5:
        drawMenu(display, shtc, pcf85063a, ITimer, (timer_callback)timer2ISR);
        delay(10);
        break;
    default:
        break;
    }
}
