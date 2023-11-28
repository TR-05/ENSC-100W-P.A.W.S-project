#pragma once
#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

class LCD
{
public:
    String topRow = "null";
    String bottomRow = "null";
    void set(String topRow, String bottomRow)
    {
        this->topRow = topRow;
        this->bottomRow = bottomRow;
    }
    void update()
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(topRow);
        lcd.setCursor(0, 1);
        lcd.print(bottomRow);
    }
};
LCD lcd1;


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
