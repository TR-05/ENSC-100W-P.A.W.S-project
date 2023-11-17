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

// include the library code:
#include <LiquidCrystal.h>
#include "IRremote.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define ENABLE 5
#define DIRA 3
#define DIRB 4
const int leftButton = 2;  // direction control button is connected to Arduino pin 4
const int rightButton = 6;  // direction control button is connected to Arduino pin 4
int receiver = 13; // Signal Pin of IR receiver to Arduino Digital Pin 6

IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

void setup() {
  //---set pin direction
  irrecv.enableIRIn(); // Start the receiver
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

//bool dir = 1;
int state_motor = 0;
int leftState = 0;
int rightState = 0;

bool last_button_VolUp = false;
bool last_button_VolDown = false;

float motor_rpm = 6372;
float output_rpm = 177;
float gear_ratio = motor_rpm / output_rpm * 1.15;
int acel_time = 0;
bool button_VolUp = false;
bool button_VolDown = false;
bool button_EQ = false, last_button_EQ = false;
bool button_power = false, button_0 = false, button_1 = false, button_2 = false;
bool last_button_power = false, last_button_0 = false, last_button_1 = false, last_button_2 = false;
int lcd_counter = 0;

void loop() {
  recieveIR();
  if (rightState !=  digitalRead(rightButton) &&  digitalRead(rightButton) == 0) {
    cycleMotor();
  }
  //testing little function
  if (last_button_EQ !=  button_EQ &&  button_EQ) {
    spinFor(360, -1);
  }

  if (last_button_power !=  button_power &&  button_power) {
    stop();
  }

  if (last_button_0 !=  button_0 &&  button_0) {
    stop();
  }
  if (last_button_1 !=  button_1 &&  button_1) {
    spinForward();
  }
  if (last_button_2 !=  button_2 &&  button_2) {
    spinBackward();
  }

  if (button_VolUp !=  last_button_VolUp &&  button_VolUp) {
    lcd_counter++;
  }

  if (button_VolDown !=  last_button_VolDown &&  button_VolDown) {
    lcd_counter--;
  }


  leftState = digitalRead(leftButton);
  rightState = digitalRead(rightButton);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter");
  lcd.setCursor(14, 0);
  lcd.print(lcd_counter);
  
  lcd.setCursor(0, 1);
  lcd.print("Left Button");
  lcd.setCursor(15, 1);
  if (leftState == 0) {
  lcd.print("1");
  }
  else {
  lcd.print("0");   
  }  




  last_button_VolDown = button_VolDown;
  last_button_VolUp = button_VolUp;
  last_button_EQ = button_EQ;
  last_button_power = button_power;
  last_button_0 = button_0;
  last_button_1 = button_1;
  last_button_2 = button_2;

  delay(20);
}

void spinFor(float deg, int dir) {
  
    stop();
    delay(200);
    if (dir == 1) {
    spinForward();
    }
    if (dir == -1) {
    spinBackward();
    }
    if (dir == 0) stop();
    int timeToSpinFor = rotationTime(deg) + acel_time;
    int startMillis = millis();
    int loopCounter = 0;
    //while(millis() - startMillis < timeToSpinFor)
    //{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(millis() - startMillis);

        delay(timeToSpinFor);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(millis() - startMillis);
    //}
    stop();
    state_motor = 0;
    delay(200);
}


int rotationTime(float degrees) 
{
  float targetRotations = degrees/360;
  float rotations_of_motor = targetRotations * gear_ratio;
  float output = motor_rpm / rotations_of_motor;
  output /= 60;
  output = 1 / output;
    Serial.print("output seconds: ");  
    Serial.print(output);  
    Serial.print(" input: ");  
    Serial.print(degrees);  
    output *= 1000;

    // prints a tab 
    Serial.println();

  return int(output);
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

void cycleMotor() 
{
    state_motor++;
    if (state_motor > 1){
      state_motor = -1;
    }

    if (state_motor == 1) {
        spinForward();
    }
    else if (state_motor == -1){
        spinBackward();
    }
    else {
      stop();
    }
}

void recieveIR()
{

if (irrecv.decode(&results)) // have we received an IR signal?

  {
    Serial.print("New Press: ");  
    Serial.print(results.value, HEX);  
    Serial.println();
  
    button_VolUp = false;
    button_VolDown= false;
    button_EQ = false;
    button_0 = false;
    button_1 = false;
    button_2 = false;
    button_power = false;

    switch(results.value)
    {


      case 0xFFA857: // VOL- button pressed
      button_VolDown = true;
                      break;
      case 0xFF629D: // VOL+ button pressed 
      button_VolUp = true;
                      break;
      case 0xFF6897: // 0
      button_0 = true;
                      break;
      case 0xFFA25D: // Power
      button_power = true;
                      break;  
      case 0xFFE21D: // FUNC/STOP
      button_VolUp = true;
                      break;  
      case 0xFF22DD: // left arrow
      button_VolUp = true;
                      break;  
      case 0xFF02FD: // Pause/Play
      button_VolUp = true;
                      break;  
      case 0xFFC23D: // Right Arrow
      button_VolUp = true;
                      break;  
      case 0xFFE01F: // Down Arrow
      button_VolUp = true;
                      break;  
      case 0xFF906F: // Up Arrow
      button_VolUp = true;
                      break;  
      case 0xFF9867: // button_EQ
      button_EQ = true;
                      break;  
      case 0xFFB04F: // ST/REPT
      button_VolUp = true;
                      break;  
      case 0xFF30CF: // 1
      button_1 = true;
                      break;  
      case 0xFF18E7: // 2
      button_2 = true;
                      break;  
      case 0xFF7A85: // 3
      button_VolUp = true;
                      break;  
      case 0xFF10EF: // 4
      button_VolUp = true;
                      break;  
      case 0xFF38C7: // 5
      button_VolUp = true;
                      break;  
      case 0xFF5AA5: // 6
      button_VolUp = true;
                      break;  
      case 0xFF42BD: // 7
      button_VolUp = true;
                      break;  
      case 0xFF4AB5: // 8
      button_VolUp = true;
                      break;  
      case 0xFF52AD: // 9
      button_VolUp = true;
                      break;  
      
    }
    
      irrecv.resume(); // receive the next value    
  }  
}