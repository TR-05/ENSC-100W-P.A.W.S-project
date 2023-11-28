
#include "IRremote.h"
#include "lcd.h"
#include "motor.h"
#include "serial.h"
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
IRrecv irrecv(receiver);  // create instance of 'irrecv'
decode_results results;   // create instance of 'decode_results'

//global
int time_interval = 10;
double food_amount;
unsigned long last_dispense_time = 0;
int set_upTime = millis();


//bool dir = 1;
int state_motor = 0;



int button_pressed = 0;
//result.value will be lcd key

float motor_rpm = 6372;
float output_rpm = 35.4;
float gear_ratio = motor_rpm / output_rpm * 1;
int acel_time = 0;

//vex limit switch
bool limit_switch, last_limit_switch;

//Remote buttons
bool button_VolUp, last_button_VolUp, button_VolDown, last_button_VolDown,
  button_EQ, last_button_EQ, button_power, last_button_power, button_0, last_button_0, button_1, last_button_1,
  button_2, last_button_2, button_Down, last_button_Down, button_Up, last_button_Up, button_Func, last_button_Func, button_Left, last_button_Left, button_Right, last_button_Right,
  last_button_ST, button_ST;
bool joystick_button, last_joystick_button;
float joystick_x, joystick_y;
//Amount
double food_out = 0;
double rotate_amount = food_amount * 4;

int lcd_counter = 0;

int user_time() {
  lcd.setCursor(0, 0);
  if (joystick_x > 0) {
    lcd_counter++;
    delay(200);
    return lcd_counter;
  }
}
//joystick_x = map(analogRead(X_pin), 0, 1023, -100, 100);
void set_time() {
  lcd.setCursor(0, 0);
  lcd.print(user_time());
}


void setup() {
  irrecv.enableIRIn();  // Start the receiver
  pinMode(limit_switch_port, INPUT_PULLUP);
  pinMode(SW_pin, INPUT_PULLUP);
  motor.initialize();
  Serial.begin(115200);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd1.set("time", "");
  lcd1.topRow = " cups:";
  lcd1.update();
  //time_interval = getTime();
  Serial.print(time_interval);
  //delay(1000);
  //food_amount = getFood();
  Serial.print(food_amount);

  lcd.print(food_amount);
  lcd.print(" cups");
  lcd.setCursor(0, 1);
  lcd.print("every ");
  lcd.print(time_interval);
  lcd.print(" minutes");
  time_interval = time_interval*(millis()/(1000* 60));
  //delay(2000);
}

void loop() {
   //unsigned long elapsed_millis = millis();
  //int elapsed_time = elapsed_millis / (1000 * 60);  // Convert milliseconds to minutes
  //Serial.print(elapsed_time);
    //dispenseFood();
  /*if (elapsed_time % time_interval == 0) {
    lcd.clear();
    lcd.print("Dispensing");
    double food_dispensed = 0;
    while (food_dispensed < food_amount) {
      spinFor(180, 1);
      delay(500);
      spinFor(180, 1);
      food_dispensed += 0.25;
    }
    if (food_amount == food_dispensed){
      lcd.clear();
      Serial.print("completed");
      lcd.print("completed");
    }
  }*/
float current_time = millis()/(1000.0);
Serial.print(set_upTime/1000);
Serial.print("Current time: ");
Serial.println(current_time - (set_upTime/1000));
//delay(1000);
int current_time_int = current_time;
if ((fmod(current_time, 5.0)) < 1) {
  lcd.clear();
  lcd.print("Dispensing");
  //while (food_amount <= food_out) {
    //spinFor(180, 1);
    //delay(500);
    //spinFor(180, 1);
    //food_out += 0.25;
    //delay(100);
  //}
  //if (food_amount == food_out){
    //lcd.clear();
   // lcd.print("complete");
  //}
  //delay(2000);
} else {
  lcd.clear();
  lcd.print("not dispensing");
}

 /* int dispense_at = elapsed_time / time_interval;
  if (elapsed_time == dispense_at) {
    lcd.clear();
    lcd.print("Dispensing");
    while (food_amount <= food_out) {
      spinFor(180, 1);
      delay(500);
      spinFor(180, 1);
      food_out += 0.25;
    }
    if (food_amount == food_out){
      lcd.print("Complete");
    }
  }*/
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


  if (joystick_y > 0) {
    float speed = map(joystick_y, 0, 100, 50, 255);
    motor.spin(speed);
  } else if (joystick_y < 0) {
    float speed = map(joystick_y, 0, -100, 50, 255);
    motor.spin(speed, true);
  } else {
    motor.spin(0);
  }

    limit_switch = !digitalRead(limit_switch_port);
  if (limit_switch != last_limit_switch && limit_switch) {
    lcd_counter++;
  }

  if (joystick_button != last_joystick_button && joystick_button) {
    motor.CycleWheel();
    lcd_counter++;
  }

  /*if (button_Up != last_button_Up && button_Up) {
    food_amount += 0.25;
    
  }

  if (button_Down != last_button_Down && button_Down && food_amount > 0)  {
    food_amount -= 0.25;
  }

  if (button_Right != last_button_Right && button_Right) {
    time_interval++;
  }

  if (button_Left != last_button_Left && button_Left && time_interval > 0) {
    time_interval--;
  }*/




  /*lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.print("Interval");
  lcd.setCursor(14, 0);
  lcd.print(time_interval);*/
 

  /*lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter");
  lcd.setCursor(14, 0);
  lcd.print(lcd_counter);




  lcd.setCursor(0, 1);
  lcd.print(joystick_x);
  lcd.setCursor(8, 1);
  lcd.print(joystick_y);*/

  /*Serial.print("Switch:  ");
  Serial.print(joystick_button);
  Serial.print("  X-axis: ");
  Serial.print(joystick_x);
  Serial.print("  Y-axis: ");
  Serial.print(joystick_y);
  Serial.print("\n");*/




  last_limit_switch = limit_switch;
  last_button_VolDown = button_VolDown;
  last_button_VolUp = button_VolUp;
  last_button_EQ = button_EQ;
  last_button_power = button_power;
  last_button_0 = button_0;
  last_button_1 = button_1;
  last_button_2 = button_2;
  last_joystick_button = joystick_button;
  last_button_Right = button_Right;
  last_button_Left = button_Left;
  last_button_ST = button_ST;
  last_button_Down = button_Down;
  last_button_Up = button_Up;
  last_button_Func = button_Func;
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
  analogWrite(motor.ENABLE, velocity);  //enable on
  digitalWrite(motor.DIRA, HIGH);       //one way
  digitalWrite(motor.DIRB, LOW);
}

void spinBackward(float velocity) {
  analogWrite(motor.ENABLE, velocity);  //enable on
  digitalWrite(motor.DIRA, LOW);        //one way
  digitalWrite(motor.DIRB, HIGH);
}
void spin(bool forward) {
  digitalWrite(motor.ENABLE, HIGH);  //enable on
  if (forward) {
    digitalWrite(motor.DIRA, HIGH);  //one way
    digitalWrite(motor.DIRB, LOW);
  } else {
    digitalWrite(motor.DIRA, LOW);  //one way
    digitalWrite(motor.DIRB, HIGH);
  }
}

void stop() {
  digitalWrite(motor.DIRA, LOW);  //fast stop
  digitalWrite(motor.DIRB, LOW);  //fast stop
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

int recieveIR() {
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
  button_Left = false;
  button_Right = false;
  button_ST = false;
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
        button_pressed = 0xFFA857;
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
        button_Left = true;
        break;
      case 0xFF02FD:  // Pause/Play
        button_VolUp = true;
        break;
      case 0xFFC23D:  // Right Arrow
        button_Right = true;
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
        button_ST = true;
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
  return 0;
}

int getTime() {
  int time_interval = 0;  // Variable to store the time interval
  int set_upTime = millis();

  while (true) {
    if (irrecv.decode(&results)) {
      switch (results.value) {
        case 0xFFC23D:
          // Increment the time interval when the right button is pressed
          time_interval++;
          break;

        case 0xFF22DD:
          // Decrement the time interval when the left button is pressed
          if (time_interval > 0) {
            time_interval--;
          }
          break;

        case 0xFFB04F:
          // Display a confirmation message when the ST button is pressed
          lcd.clear();
          lcd.print("Press ST ");
          lcd.setCursor(0, 1);
          lcd.print("to confirm");
          irrecv.resume();  // Receive the next value

          // Wait for the next ST button press
          while (true) {
            if (irrecv.decode(&results)) {
              if (results.value == 0xFFB04F) {
                // Return the time interval when ST button is pressed again
                return time_interval;
              }
              irrecv.resume();  // Receive the next value
            }
          }
          break;

        default:
          break;
      }
      irrecv.resume();  // Receive the next value
    }

    // Display the current time interval
    lcd.clear();
    lcd.print("Time: ");
    lcd.print(time_interval);
    lcd.print(" minutes");
  }
  return time_interval;
}

double getFood() {
  double food_amount = 0;  // Variable to store the food amount

  while (true) {
    if (irrecv.decode(&results)) {
      switch (results.value) {
        case 0xFF906F:  //button up
          // Increment the food amount when the up button is pressed
          food_amount += 0.25;
          break;

        case 0xFFE01F:
          // Decrement the food amount when the down button is pressed by 1/4 cup
          if (food_amount > 0) {
            food_amount -= .25;
          }
          break;

        case 0xFFE21D:
          // Display a confirmation message when the Func button is pressed
          lcd.clear();
          lcd.print("Press Func ");
          lcd.setCursor(0, 1);
          lcd.print("to confirm");
          irrecv.resume();  // Receive the next value

          // Wait for the next func button press
          while (true) {
            if (irrecv.decode(&results)) {
              if (results.value == 0xFFE21D) {
                // Return the food when Func button is pressed again
                return food_amount;
              }
              irrecv.resume();  // Receive the next value
            }
          }
          break;
      }
      irrecv.resume();  // Receive the next value
    }

    // Display the current food amount
    lcd.clear();
    lcd.print("Food: ");
    lcd.print(food_amount);
    lcd.print(" cups");
  }
}


/*int time_elapsed() {
  int elapsed_millis = millis();
  int seconds = elapsed_millis / (1000);
  int minutes = elapsed_millis / (1000 * 60);
  int hours = elapsed_millis / (1000 * 60 * 60);

  // Print the elapsed time
  Serial.print("Elapsed time: ");
  Serial.print(minutes);
  Serial.println(" minutes");
  Serial.print(hours);
  Serial.println(" hours");

  return minutes;
}*/

void dispenseFood() {
  unsigned long current_time = millis()/(1000);
  delay(1000);
  Serial.print(set_upTime/1000);
  Serial.print("Current time: ");
  Serial.println(current_time - (set_upTime/1000));
  /*Serial.print("Last dispense time: ");
  Serial.println(last_dispense_time);

  unsigned long elapsed_time = (current_time - last_dispense_time) / (1000 * 60);  // Convert milliseconds to minutes
  int remaining_time = time_interval - elapsed_time;

  Serial.print("Elapsed time: ");
  Serial.println(elapsed_time);
  Serial.print("Remaining time: ");
  Serial.println(remaining_time);
  if (remaining_time <= 0) {

  } else {
    // Print remaining time to LCD and Serial monitor
    // ...
  }*/
}




