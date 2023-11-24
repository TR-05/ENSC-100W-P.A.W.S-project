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
const int SW_pin = 6;             // digital pin connected to switch output
const int X_pin = A0;             // analog pin connected to X output
const int Y_pin = A1;             // analog pin connected to Y output
const int limit_switch_port = 2;  // direction control button is connected to Arduino pin 4
//const int rightButton = 13;        // direction control button is connected to Arduino pin 4
const int receiver = 13;  // Signal Pin of IR receiver to Arduino Digital Pin 6

IRrecv irrecv(receiver);  // create instance of 'irrecv'
decode_results results;   // create instance of 'decode_results'

void setup() {
  irrecv.enableIRIn();  // Start the receiver
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(limit_switch_port, INPUT_PULLUP);
  pinMode(SW_pin, INPUT_PULLUP);

  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("time");
}

//bool dir = 1;
int state_motor = 0;

float motor_rpm = 6372;
float output_rpm = 35.4;
float gear_ratio = motor_rpm / output_rpm * 1;
int acel_time = 0;

//vex limit switch
bool limit_switch, last_limit_switch;

//Remote buttons
bool button_VolUp, last_button_VolUp, button_VolDown, last_button_VolDown,
  button_EQ, last_button_EQ, button_power, last_button_power, button_0, last_button_0, button_1, last_button_1,
  button_2, last_button_2, button_Down, last_button_Down, button_Up, last_button_Up, button_Func, last_button_Func;
bool joystick_button, last_joystick_button;
float joystick_x, joystick_y;
//Amount
float cup_amount = 0;
float user_amount = 0;
float rotate_amount = user_amount * 4;

int lcd_counter = 0;

void loop() {
  //Serial.print("Time: ");
  float t = millis();
  t /= 1000;
  //Serial.print(t, 1);
  //Serial.println();
  joystick_button = !digitalRead(SW_pin);
  joystick_x = map(analogRead(X_pin), 0, 1023, -100, 100);
  joystick_y = map(analogRead(Y_pin), 0, 1023, -100, 100);
  if (fabs(joystick_x) < 6) joystick_x = 0;
  if (fabs(joystick_y) < 6) joystick_y = 0;

  recieveIR();
  //testing little function
  if (last_button_EQ != button_EQ && button_EQ) {
    spinFor(360, -1);
  }

  if (last_button_power != button_power && button_power) {
    stop();
  }

  if (last_button_0 != button_0 && button_0) {
    stop();
  }
  if (last_button_1 != button_1 && button_1) {
    spinForward(255);
  }
  if (last_button_2 != button_2 && button_2) {
    spinBackward(255);
  }

  if (button_Up != last_button_Up && button_Up) {
    lcd_counter++;
    //Serial.println(lcd_counter);
  }

    if (joystick_y > 0) 
    {
    float speed = map(joystick_y, 0, 100, 50, 255);
    spinForward(speed);
    } 
    else if (joystick_y < 0)
    {
    float speed = map(joystick_y, 0, -100, 50, 255);
      spinBackward(speed);
    }
    else {
      stop();
    }
  if (joystick_button != last_joystick_button && joystick_button) {
    spinFor(180, 1);
    delay(500);
    spinFor(180, 1);
    lcd_counter++;
  }

  if (button_Down != last_button_Down && button_Down) {
    lcd_counter--;
  }


  limit_switch = !digitalRead(limit_switch_port);
  if (limit_switch != last_limit_switch && limit_switch) {
    spinFor(180, 1);
    delay(500);
    spinFor(180, 1);
    lcd_counter++;
    //Serial.println(lcd_counter);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter");
  lcd.setCursor(14, 0);
  lcd.print(lcd_counter);




  lcd.setCursor(0, 1);
  lcd.print(joystick_x);
  lcd.setCursor(8, 1);
  lcd.print(joystick_y);

  Serial.print("Switch:  ");
  Serial.print(joystick_button);
  Serial.print("  X-axis: ");
  Serial.print(joystick_x);
  Serial.print("  Y-axis: ");
  Serial.print(joystick_y);
  Serial.print("\n");


  last_limit_switch = limit_switch;
  last_button_VolDown = button_VolDown;
  last_button_VolUp = button_VolUp;
  last_button_EQ = button_EQ;
  last_button_power = button_power;
  last_button_0 = button_0;
  last_button_1 = button_1;
  last_button_2 = button_2;
  last_joystick_button = joystick_button;
  delay(20);
}

void spinFor(float deg, int dir) {

  stop();
  delay(200);
  if (dir == 1) {
    spinForward(255);
  }
  if (dir == -1) {
    spinBackward(255);
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


int rotationTime(float degrees) {
  float targetRotations = degrees / 360;
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
void spinForward(float velocity) {
  analogWrite(ENABLE,velocity); //enable on
  digitalWrite(DIRA, HIGH);    //one way
  digitalWrite(DIRB, LOW);
}

void spinBackward(float velocity) {
  analogWrite(ENABLE,velocity); //enable on
  digitalWrite(DIRA, LOW);     //one way
  digitalWrite(DIRB, HIGH);
}
void spin(bool forward) {
  digitalWrite(ENABLE, HIGH);  //enable on
  if (forward) {
    digitalWrite(DIRA, HIGH);  //one way
    digitalWrite(DIRB, LOW);
  } else {
    digitalWrite(DIRA, LOW);  //one way
    digitalWrite(DIRB, HIGH);
  }
}

void stop() {
  digitalWrite(DIRA, LOW);  //fast stop
  digitalWrite(DIRB, LOW);  //fast stop
}

void cycleMotor() {
  state_motor++;
  if (state_motor > 1) {
    state_motor = -1;
  }

  if (state_motor == 1) {
    spinForward(255);
  } else if (state_motor == -1) {
    spinBackward(255);
  } else {
    stop();
  }
}

void recieveIR() {
  button_VolUp = false;
  button_VolDown = false;
  button_EQ = false;
  button_0 = false;
  button_1 = false;
  button_2 = false;
  button_power = false;
  button_Down = false;
  button_Up = false;
  button_Func = false;

  if (irrecv.decode(&results))  // have we received an IR signal?

  {
    Serial.print("New Press: ");
    Serial.print(results.value, HEX);
    Serial.println();


    switch (results.value) {
        // case 0xFFFFFFFF:
        //   break;

      case 0xFFA857:  // VOL- button pressed
        button_VolDown = true;
        break;
      case 0xFF629D:  // VOL+ button pressed
        button_VolUp = true;
        break;
      case 0xFF6897:  // 0
        button_0 = true;
        break;
      case 0xFFA25D:  // Power
        button_power = true;
        break;
      case 0xFFE21D:  // FUNC/STOP
        button_Func = true;
        break;
      case 0xFF22DD:  // left arrow
        button_VolUp = true;
        break;
      case 0xFF02FD:  // Pause/Play
        button_VolUp = true;
        break;
      case 0xFFC23D:  // Right Arrow
        button_VolUp = true;
        break;
      case 0xFFE01F:  // Down Arrow
        button_Down = true;
        break;
      case 0xFF906F:  // Up Arrow
        button_Up = true;
        break;
      case 0xFF9867:  // button_EQ
        button_EQ = true;
        break;
      case 0xFFB04F:  // ST/REPT
        button_VolUp = true;
        break;
      case 0xFF30CF:  // 1
        button_1 = true;
        break;
      case 0xFF18E7:  // 2
        button_2 = true;
        break;
      case 0xFF7A85:  // 3
        button_VolUp = true;
        break;
      case 0xFF10EF:  // 4
        button_VolUp = true;
        break;
      case 0xFF38C7:  // 5
        button_VolUp = true;
        break;
      case 0xFF5AA5:  // 6
        button_VolUp = true;
        break;
      case 0xFF42BD:  // 7
        button_VolUp = true;
        break;
      case 0xFF4AB5:  // 8
        button_VolUp = true;
        break;
      case 0xFF52AD:  // 9
        button_VolUp = true;
        break;
    }

    irrecv.resume();  // receive the next value
  }
}

/*void feeder(rotate_amount){
  do{
      spinFor(180, 1);
      stop(1000);
      spinFor(180, 1);
      cup_amount += .25;
    } while (cup_amount < user_amount);
}*/