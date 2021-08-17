#line 1 "/Users/nitkonitkic/Documents/Code/Weather_Station_And_Clock/Screen_Menu.h"
#include "BME280-SOLDERED.h"
#include "PCF85063A-SOLDERED.h"
#include "OLED-Display-SOLDERED.h"

#include "Helpers.h"

bool pressed = false;
uint8_t menu_state = 0;

void menuIncrement()
{
    pressed = true;
}

void menuStateCallback()
{
    menu_state = (menu_state + 1) % 14;
}

void drawBlinky(OLED_Display &display, uint8_t *settings, int n, const char *indexed[] = nullptr)
{
    if (indexed == nullptr)
        display.print((char)(millis() & 512 && menu_state == n ? ' ' : settings[n] + '0'));
    else
    {
        char a[16];
        strcpy_P(a, (PGM_P)pgm_read_word(&(indexed[settings[n]])));
        if (millis() & 512 && menu_state == n)
            memset(a, ' ', strlen(a));
        display.print(a);
    }
}

bool validTime(uint8_t *settings)
{
    int hours = 10 * settings[0] + settings[1],
        minutes = 10 * settings[2] + settings[3],
        seconds = 10 * settings[4] + settings[5],
        day = 10 * settings[6] + settings[7],
        month = 10 * settings[8] + settings[9],
        year = 2000 + 10 * settings[10] + settings[11],
        weekDay = settings[12];

    unsigned int leap;
    unsigned char mon_day[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year < 1) || (year >= 3200))
    {
        return 0;
    }
    if ((month < 1) || (month > 12))
    {
        return 0;
    }
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
    {
        leap = 1;
    }
    else
    {
        leap = 0;
    }

    mon_day[1] += leap;
    if ((day > mon_day[month - 1]) || (day < 1))
    {
        return 0;
    }

    if (weekDay > 6 || weekDay < 0)
    {
        return 0;
    }

    if (hours < 0 || hours > 23)
    {
        return 0;
    }

    if (minutes < 0 || minutes > 59)
    {
        return 0;
    }

    return 1;
}

void drawMenu(OLED_Display &display,
              BME280 &bme280,
              PCF85063A &pcf85063a)
{
    resetText(display);
    display.clearDisplay();

    int secs = pcf85063a.getSecond(),
        mins = pcf85063a.getMinute(),
        hours = pcf85063a.getHour(),
        days = pcf85063a.getDay(),
        months = pcf85063a.getMonth(),
        year = pcf85063a.getYear(),
        weekDay = pcf85063a.getWeekday();

    uint8_t settings[] = {hours / 10, hours % 10,
                          mins / 10, mins % 10,
                          secs / 10, secs % 10,
                          days / 10, days % 10,
                          months / 10, months % 10,
                          year / 10 % 10, year % 10,
                          weekDay, getTimeZone()};

    if (pressed)
    {
        settings[menu_state] = (settings[menu_state] + 1) % (menu_state == 13 ? 24 : 10);

        for (int i = 0; !validTime(settings) && i < 100; ++i)
        {
            settings[menu_state] = (settings[menu_state] + 1) % (menu_state == 13 ? 24 : 10);
        }
    }

    pressed = 0;
    pcf85063a.setTime(settings[0] * 10 + settings[1],
                      settings[2] * 10 + settings[3],
                      settings[4] * 10 + settings[5]);
    pcf85063a.setDate(settings[12], settings[6] * 10 + settings[7], settings[8] * 10 + settings[9], 2000 + settings[10] * 10 + settings[11]);

    display.setCursor(5, 10);
    display.print("Set time: ");

    drawBlinky(display, settings, 0);
    drawBlinky(display, settings, 1);
    display.print(":");
    drawBlinky(display, settings, 2);
    drawBlinky(display, settings, 3);
    display.print(":");
    drawBlinky(display, settings, 4);
    drawBlinky(display, settings, 5);

    display.setCursor(5, 20);
    display.print("Set date: ");
    drawBlinky(display, settings, 6);
    drawBlinky(display, settings, 7);
    display.print(".");
    drawBlinky(display, settings, 8);
    drawBlinky(display, settings, 9);
    display.print(".");
    drawBlinky(display, settings, 10);
    drawBlinky(display, settings, 11);

    display.setCursor(5, 30);
    display.print("Set weekday: ");
    drawBlinky(display, settings, 12, weekDays);

    display.setCursor(5, 40);
    display.print("Set timezone: +");
    if (millis() & 512 && menu_state == 13)
        display.print(" ");
    else
        display.print((int)settings[13]);

    setTimeZone(settings[13]);

    display.display();
}
