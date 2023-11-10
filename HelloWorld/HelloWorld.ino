//www.elegoo.com
//2016.12.9

/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

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

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define ENABLE 5
#define DIRA 3
#define DIRB 4
const int leftButton = 2;  // direction control button is connected to Arduino pin 4
const int rightButton = 6;  // direction control button is connected to Arduino pin 4
void setup() {
  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("time");
}

bool dir = 1;
int leftState = 0;
int rightState = 0;
void loop() {

  if (rightState !=  digitalRead(rightButton) &&  digitalRead(rightButton) == 0) {
    stop();
    delay(200);
    if (dir) {
        spinForward();
    }
    else {
        spinBackward();
    }
    dir = !dir;
  }
  if (leftState !=  digitalRead(leftButton) &&  digitalRead(leftButton) == 0) {
    stop();
  }
  leftState = digitalRead(leftButton);
  rightState = digitalRead(rightButton);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Left Button");
  lcd.setCursor(15, 0);
  if (leftState == 0) {
  lcd.print("P");
  }
  else {
  lcd.print("Q");   
  }
  lcd.setCursor(0, 1);
  lcd.print("Right Button");
  lcd.setCursor(15, 1);
  if (rightState == 0) {
  lcd.print("P");
  }
  else {
  lcd.print("Q");   
  }  
  /*
  print("Forward");
  spinForward();
  delay(3000);

  print("Stop");
  stop();
  delay(2000);

  print("Backward");
  spinBackward();
  delay(3000);

  print("Stop");
  stop();
  delay(2000);*/
  delay(50);
}

void print(String display) {
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(millis() / 1000);
  lcd.setCursor(0, 1);
  lcd.print(display);
}
void spinForward() {
  digitalWrite(ENABLE,HIGH); //enable on
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
}

void spinBackward() {
  digitalWrite(ENABLE,HIGH); //enable on
  digitalWrite(DIRA,LOW); //one way
  digitalWrite(DIRB,HIGH);
}
void spin(bool forward) {
  digitalWrite(ENABLE,HIGH); //enable on
  if (forward) {
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  }
  else {
  digitalWrite(DIRA,LOW); //one way
  digitalWrite(DIRB,HIGH);
  }
}

void stop() {
  digitalWrite(DIRA,LOW); //fast stop
  digitalWrite(DIRB,LOW); //fast stop  
}
