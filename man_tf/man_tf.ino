
#include "lcd.h"
#include "motor.h"
#include "serial.h"
#include "button.h"
#include "foodDispensing.h"
#include "ir.h"
#include "time.h"
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// global
float joystick_x, joystick_y;
int lcd_counter = 0;
int user_time()
{
  lcd.setCursor(0, 0);
  if (joystick_x > 0)
  {
    lcd_counter++;
    delay(200);
    return lcd_counter;
  }
}
// joystick_x = map(analogRead(X_pin), 0, 1023, -100, 100);
void set_time()
{
  lcd.setCursor(0, 0);
  lcd.print(user_time());
}

button limit_switch(limit_switch_port);
button joystick(joystick_button_port);

void initalizeDispenser() {
  Serial.print(dispense::getTime());
  Serial.print("\n\n");
  delay(2000);
  Serial.print(dispense::getFoodAmount());
  Serial.print("\n\n");

  LCD::print("     Cups", "every      Min");
  LCD::print(0,0, dispense::food_amount, 2);
  LCD::print(6,1, dispense::time_interval, 2);
  delay(3000);
  time::set_up_time_ms = millis() - 1001;
}
void setup()
{
  Serial.begin(115200);
  motor.initialize();
  IR::intialize();
  LCD::initialize();
  initalizeDispenser();
  limit_switch.newPress();
  if (limit_switch.currentState == false) {
    motor.CycleWheel(255);
  }
}

void loop()
{
  IR::recieveIR();
  LCD::clear();
  time::update();
  LCD::print(0,0, time::time_ms, 0);
  LCD::print(8,0, time::time_s, 1);
  LCD::print(0,1, time::time_m, 2);
  LCD::print(8,1, time::time_h, 3);

  if (IR::func.newPress(true)) {
    initalizeDispenser();
  }

  time::checkForDispense(dispense::time_interval, 255, dispense::food_amount);

  joystick_x = map(analogRead(X_pin), 0, 1023, -100, 100);
  joystick_y = map(analogRead(Y_pin), 0, 1023, -100, 100);
  if (fabs(joystick_x) < 6)
    joystick_x = 0;
  if (fabs(joystick_y) < 6)
    joystick_y = 0;

  // testing little function
  if (IR::EQ.newPress(true))
  {
    motor.spinFor(180, 255, 1);
  }

  if (IR::power.newPress(true))
  {
    motor.spin(0);
  }

  if (IR::_0.newPress(true))
  {
    motor.spin(0);
  }
  if (IR::_1.newPress(true))
  {
    motor.spin(255, false);
  }
  if (IR::_2.newPress(true))
  {
    motor.spin(255, true);
  }

  if (joystick_y > 0)
  {
    float speed = map(joystick_y, 0, 100, 50, 255);
    motor.spin(speed);
  }
  else if (joystick_y < 0)
  {
    float speed = map(joystick_y, 0, -100, 50, 255);
    motor.spin(speed, true);
  }
  else
  {
    motor.spin(0);
  }

  if (limit_switch.newPress())
  {
    lcd_counter++;
  }

  if (joystick.newPress())
  {
    motor.CycleWheel(255);
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

  delay(20);
}

/*
double getFood()
{
  double food_amount = 0; // Variable to store the food amount

  while (true)
  {
    if (irrecv.decode(&results))
    {
      switch (results.value)
      {
      case 0xFF906F: // button up
        // Increment the food amount when the up button is pressed
        food_amount += 0.25;
        break;

      case 0xFFE01F:
        // Decrement the food amount when the down button is pressed by 1/4 cup
        if (food_amount > 0)
        {
          food_amount -= .25;
        }
        break;

      case 0xFFE21D:
        // Display a confirmation message when the Func button is pressed
        lcd.clear();
        lcd.print("Press Func ");
        lcd.setCursor(0, 1);
        lcd.print("to confirm");
        irrecv.resume(); // Receive the next value

        // Wait for the next func button press
        while (true)
        {
          if (irrecv.decode(&results))
          {
            if (results.value == 0xFFE21D)
            {
              // Return the food when Func button is pressed again
              return food_amount;
            }
            irrecv.resume(); // Receive the next value
          }
        }
        break;
      }
      irrecv.resume(); // Receive the next value
    }

    // Display the current food amount
    lcd.clear();
    lcd.print("Food: ");
    lcd.print(food_amount);
    lcd.print(" cups");
  }
}
*/