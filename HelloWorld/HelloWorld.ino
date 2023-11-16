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

bool lastVolUp = false;
bool lastVolDown = false;

float motor_rpm = 6372;
float output_rpm = 177;
float gear_ratio = motor_rpm / output_rpm * 1.15;
int acel_time = 0;
bool currentVolUp = false;
bool currentVolDown = false;
bool EQ = false, lastEQ = false;
void loop() {
  recieveIR();
  if (rightState !=  digitalRead(rightButton) &&  digitalRead(rightButton) == 0) {
    //stop();
    //delay(200);
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
  //testing little function
  if (lastEQ !=  EQ &&  EQ) {
    spinFor(360, -1);
    //spinFor(720);
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






   
  if (currentVolUp !=  lastVolUp &&  currentVolUp) {
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
  lastVolUp = currentVolUp;


  if (currentVolDown !=  lastVolDown &&  currentVolDown) {
    /*
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
    */
    state_motor = 0;
    stop();
  }
  lastVolDown = currentVolDown;


  lastEQ = EQ;

  delay(50);
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



void recieveIR()
{

if (irrecv.decode(&results)) // have we received an IR signal?

  {
    Serial.print("New Press: ");  
    Serial.print(results.value, HEX);  
    Serial.println();
  
    currentVolUp = false;
    currentVolUp = false;
    EQ = false;
    switch(results.value)
    {


      case 0xFFA857: // VOL- button pressed
      currentVolUp = true;
                      break;
      case 0xFF629D: // VOL+ button pressed 
      currentVolUp = true;
                      break;
      case 0xFF6897: // 0
      currentVolUp = true;
                      break;
      case 0xFFA25D: // Power
      currentVolDown = true;
                      break;  
      case 0xFFE21D: // FUNC/STOP
      currentVolUp = true;
                      break;  
      case 0xFF22DD: // left arrow
      currentVolUp = true;
                      break;  
      case 0xFF02FD: // Pause/Play
      currentVolUp = true;
                      break;  
      case 0xFFC23D: // Right Arrow
      currentVolUp = true;
                      break;  
      case 0xFFE01F: // Down Arrow
      currentVolUp = true;
                      break;  
      case 0xFF906F: // Up Arrow
      currentVolUp = true;
                      break;  
      case 0xFF9867: // EQ
      EQ = true;
                      break;  
      case 0xFFB04F: // ST/REPT
      currentVolUp = true;
                      break;  
      case 0xFF30CF: // 1
      currentVolUp = true;
                      break;  
      case 0xFF18E7: // 2
      currentVolUp = true;
                      break;  
      case 0xFF7A85: // 3
      currentVolUp = true;
                      break;  
      case 0xFF10EF: // 4
      currentVolUp = true;
                      break;  
      case 0xFF38C7: // 5
      currentVolUp = true;
                      break;  
      case 0xFF5AA5: // 6
      currentVolUp = true;
                      break;  
      case 0xFF42BD: // 7
      currentVolUp = true;
                      break;  
      case 0xFF4AB5: // 8
      currentVolUp = true;
                      break;  
      case 0xFF52AD: // 9
      currentVolUp = true;
                      break;  
      
    }
    
      irrecv.resume(); // receive the next value    
  }  
}