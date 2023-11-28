#pragma once
#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

namespace LCD
{
    String topRow = "null";
    String bottomRow = "null";

    void initialize() {
          lcd.begin(16, 2);
    }

    void set(String TopRow, String BottomRow)
    {
        topRow = TopRow;
        bottomRow = BottomRow;
    }
    void clear() {
        lcd.clear();
    }
    void update()
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(topRow);
        lcd.setCursor(0, 1);
        lcd.print(bottomRow);
    }
    void print(String TopRow, String BottomRow)
    {
        topRow = TopRow;
        bottomRow = BottomRow;
        update();
    }
    void print(int cursorColumn, int cursorRow, float value, int digits)
    {
        lcd.setCursor(cursorColumn, cursorRow);
        lcd.print(value, digits);
    }
}

/*
  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */
