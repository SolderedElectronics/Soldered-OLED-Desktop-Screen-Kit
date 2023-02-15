/*  Weather_Station_And_Clock

    You have to install the following libraries:

    - ESP8266TimerInterrupt: https://github.com/khoih-prog/ESP8266TimerInterrupt
    - OLED-Display-SOLDERED: https://github.com/SolderedElectronics/Soldered-OLED-Display-Arduino-Library
    - PCF85063A-SOLDERED: https://github.com/SolderedElectronics/Soldered-PCF85063A-RTC-Module-Arduino-Library
    - SHTC3-SOLDERED: https://github.com/SolderedElectronics/Soldered-SHTC3-Temperature-Humidity-Sensor-Arduino-Library
    - avdweb_Switch: https://github.com/avandalen/avdweb_Switch


    Arduino settings:
    - Board: ESP8266 (default settings)
    - Tested on version: 1.8.13
*/

// Include needed libraries
#include "EEPROM.h"
#include "ESP8266TimerInterrupt.h"
#include "OLED-Display-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "SHTC3-SOLDERED.h"
#include "avdweb_Switch.h"

// Include header files for each page on the screen
#include "Screen_Analog_Clock.h"
#include "Screen_Digital_Clock.h"
#include "Screen_Menu.h"
#include "Screen_Stopwatch.h"
#include "Screen_Timer.h"
#include "Screen_World_Clock.h"

// Include header file for the helper functions
#include "Helpers.h"

// Create needed objects
OLED_Display display;
SHTC3 shtc;
PCF85063A pcf85063a;
ESP8266Timer ITimer;
Switch btn1(12);
Switch btn2(13);

// Variables to navigate through pages
uint8_t state = 1, lastState = 0;

// Variables for button presses
bool btn1Single, btn1Double, btn2Pushed, btn2Single, btn2Double;

// The function that will be called after the single click of button 1
void btn1CallbackSingle(void *s)
{
    btn1Single = 1;
}

// The function that will be called after the double click of button 1
void btn1CallbackDouble(void *s)
{
    btn1Double = 1;
}

// The function that will be called after pushing button 2
void btn2CallbackPushed(void *s)
{
    btn2Pushed = 1;
}

// The function that will be called after the single click of button 2
void btn2CallbackSingle(void *s)
{
    btn2Single = 1;
}

// The function that will be called after the double click of button 2
void btn2CallbackDouble(void *s)
{
    btn2Double = 1;
}

// Poll the switches every xxxx milliseconds to update their status
void timer2ISR()
{
    btn1.poll();
    btn2.poll();
}

void setup()
{
    worldClockCallback();
    pinMode(14, OUTPUT);

    // Set the interval for polling buttons to 1 ms (1000 microseconds)
    ITimer.attachInterruptInterval(1000, timer2ISR);

    // Define functions that will be called on button events
    btn1.setSingleClickCallback(&btn1CallbackSingle, (void *)"0");
    btn1.setDoubleClickCallback(&btn1CallbackDouble, (void *)"0");

    btn2.setSingleClickCallback(&btn2CallbackSingle, (void *)"0");
    btn2.setDoubleClickCallback(&btn2CallbackDouble, (void *)"0");
    btn2.setPushedCallback(&btn2CallbackPushed, (void *)"0");

    // Init sensor, RTC and display
    shtc.begin();
    pcf85063a.begin();
    display.begin();

    // Init EEPROM and set default flag if it's not
    EEPROM.begin(16);
    checkEEPROM();

    // Wait a bit
    delay(2000);
}

void loop()
{
    // Check if button 1 is pressed once (Single click)
    if (btn1Single)
    {
        // Go to the next page and after passing all, go from 0 again
        if (state != 5)
            state = (state + 1) % 5;

        // Reset the button click variable
        btn1Single = 0;
    }

    // Check if button 1 is pressed twice (Double click)
    if (btn1Double)
    {
        // Check if the settings menu is activated
        if (state == 5)
            // Set the state to exit from the menu
            state = lastState;
        else
        {
            // Set the state to enter the menu
            lastState = state;
            state = 5;
        }

        // Reset the button click variable
        btn1Double = 0;
    }

    // Check if the button 2 is pushed
    if (btn2Pushed)
    {
        // If the current page is a stopwatch, start or stop the stopwatch
        if (state == 4)
            stopwatchStateCallback();

        // Reset the button push variable
        btn2Pushed = 0;
    }

    // Check if button 2 is pressed once (Single click)
    if (btn2Single)
    {
        // If the current page is the analog clock, add or remove the flag to show temperature and humidity with clock
        if (state == 0)
            analogClockCallback();

        // If the current page is the digital clock, add or remove the flag to show temperature and humidity with clock
        else if (state == 1)
            digitalClockCallback();

        // If the current page is the timer, start or stop the timer or set the timer if it's in the set mode
        else if (state == 2)
            timerStateCallback();

        // If the current page is settings, increment the number on the current position
        else if (state == 5)
            menuIncrement();

        // Reset the button click variable
        btn2Single = 0;
    }

    // Check if button 2 is pressed twice (Double click)
    if (btn2Double)
    {
        // If the current page is the timer, go to the set timer mode
        if (state == 2)
            timerSetupCallback();
        
        // If the current page is world clock, change the cities
        else if (state == 3)
            worldClockCallback();

        // If the current page is the stopwatch, reset the stopwatch
        else if (state == 4)
            stopwatchSetupCallback();

        // If the current page is settings, go to the next digit to set it
        else if (state == 5)
            menuStateCallback();

        // Reset the button click variable
        btn2Double = 0;
    }

    checkTimer(display, shtc, pcf85063a);

    // Display the current page on the screen
    switch (state)
    {
    case 0: // Analog clock page
        drawAnalogClock(display, shtc, pcf85063a);
        delay(100);
        break;
    case 1: // Digital clock page
        drawDigitalClock(display, shtc, pcf85063a);
        delay(100);
        break;
    case 2: // Timer page
        drawTimer(display, shtc, pcf85063a, 0);
        delay(10);
        break;
    case 3: // World clock page
        drawWorldClock(display, shtc, pcf85063a);
        delay(10);
        break;
    case 4: // Stopwatch page
        drawStopwatch(display, shtc, pcf85063a);
        delay(10);
        break;
    case 5: // Menu page
        drawMenu(display, shtc, pcf85063a, ITimer, (timer_callback)timer2ISR);
        delay(10);
        break;
    default: // Nothing happens if the page is not found
        break;
    }
}
