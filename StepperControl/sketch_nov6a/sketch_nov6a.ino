
#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 32

Stepper stepper(STEPS, 8, 10, 9, 11);

const int user_input_button = 4;  // direction control button is connected to Arduino pin 4
const int position_reset_button = 6;  // direction control button is connected to Arduino pin 4

const int pot = A0;    // speed control potentiometer is connected to analog pin 0

float position = 0;
float target_angle = 0;
float stepsToDeg() {
  return position * 360/32;
}
void setup() {
  // configure button pin as input with internal pull up enabled
  Serial.begin(9600); // open the serial port at 9600 bps:
  pinMode(user_input_button, INPUT_PULLUP);
  pinMode(position_reset_button, INPUT_PULLUP);
}

int direction_ = 1, speed_ = 0;
int last_user_input = 0;
void loop() {
  int user_input = digitalRead(user_input_button);
  if (user_input == 0) {
    target_angle += 90;
  }
  last_user_input = user_input;

  // read analog value from the potentiometer
  int val = analogRead(pot);

  // map digital value from [0, 1023] to [2, 500]
  // ===> min speed = 2 and max speed = 500 rpm
  if (speed_ != map(val, 0, 1023, 2, 500)) {  // if the speed was changed
    speed_ = map(val, 0, 1023, 2, 500);
    // set the speed of the motor
    stepper.setSpeed(speed_);
  }
  if (digitalRead(position_reset_button)) 
  position = 0;
  // move the stepper motor
  stepper.step(direction_);
  position += direction_;
  print();
  
}

void print() {
    // print labels
  Serial.print(stepsToDeg());  // prints a label
  Serial.print(" degrees");         // prints a tab
  Serial.println();        // carriage return after the last label
}

// a small function for button debounce
bool debounce() {
  byte count = 0;
  for (byte i = 0; i < 5; i++) {
    if (digitalRead(user_input_button) == 0)
      count++;
    delay(10);
  }
  if (count > 2) return 1;
  else return 0;
}

// a small function for button debounce
bool resetCheck() {
  byte count = 0;
  for (byte i = 0; i < 5; i++) {
    if (digitalRead(position_reset_button) == 0)
      count++;
    delay(10);
  }
  if (count > 2) return 1;
  else return 0;
}