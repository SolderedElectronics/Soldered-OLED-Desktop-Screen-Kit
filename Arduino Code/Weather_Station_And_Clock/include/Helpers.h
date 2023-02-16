#pragma once

// Default EEPROM flag when the time zone is stored
#define DEFAULT_EEPROM_FLAG 0xABCDEFED

// Draw a line on the display from point A to point B
void drawPolarLine(OLED_Display &display, int x, int y, float phi, int from, int to, float a = 1.0f, float b = 1.0f)
{
    // Calculate the start and end points
    int x1 = x + (int)(cos(phi) * a * (float)from);
    int y1 = y + (int)(sin(phi) * b * (float)from);
    int x2 = x + (int)(cos(phi) * a * (float)to);
    int y2 = y + (int)(sin(phi) * b * (float)to);

    // Draw the line on the screen
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
}

// Reset the text settings - default font, size 1, white color, and text wrap
void resetText(OLED_Display &display)
{
    display.setFont();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextWrap(true);
}

// Pattern for ----------------------------
uint8_t pattern[8] = {
    0b10001000, 0b00000000, 0b00100010, 0b00000000, 0b10001000, 0b00000000, 0b00100010, 0b00000000,
};

// Days in the week
const char week_0[] = "Sun";
const char week_1[] = "Mon";
const char week_2[] = "Tue";
const char week_3[] = "Wed";
const char week_4[] = "Thu";
const char week_5[] = "Fri";
const char week_6[] = "Sat";

// Initialize days in the week
const char *weekDays[] = {
    week_0, week_1, week_2, week_3, week_4, week_5, week_6,
};

// Set the time zone and store it in the EEPROM
void setTimeZone(uint8_t timeZone)
{
    uint32_t flag = DEFAULT_EEPROM_FLAG;
    int address = 0;

    EEPROM.put(address, flag);
    address += sizeof(flag);

    EEPROM.put(address, timeZone);

    EEPROM.commit();
}

// Read the time zone from the EEPROM or set it to 0 if it's not set
int getTimeZone()
{
    uint32_t flag;
    int address = 0;

    EEPROM.get(address, flag);
    address += sizeof(flag);

    if (flag == DEFAULT_EEPROM_FLAG)
    {
        uint8_t r;
        EEPROM.get(address, r);
        return r;
    }

    setTimeZone(0);
    return 0;
}

// Check if set the flag in EEPROM. If not, set it to the default value and set the default time zone (0)
void checkEEPROM()
{
    uint8_t defaultTimeZone = 0;
    uint32_t flag = 0;
    int address = 0;

    EEPROM.get(address, flag);
    if (flag != DEFAULT_EEPROM_FLAG)
    {
        EEPROM.put(address, DEFAULT_EEPROM_FLAG);
        address += sizeof(flag);
        EEPROM.put(address, defaultTimeZone);
        EEPROM.commit();
    }
}
