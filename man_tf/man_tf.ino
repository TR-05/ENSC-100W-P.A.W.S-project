
#include "lcd.h"
#include "motor.h"
#include "serial.h"
#include "button.h"
#include "foodDispensing.h"
#include "ir.h"
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
void setup()
{
  Serial.begin(115200);
  motor.initialize();
  IR::intialize();
  LCD::initialize();
  Serial.print(dispense::getTime());
  Serial.print("\n\n");
  LCD::print("Confirmed", "     Minutes");
  LCD::print(0, 1, dispense::time_interval);


  LCD::set("time", "");
  LCD::topRow = " cups:";
  LCD::update();
  Serial.print(dispense::time_interval);
  // delay(1000);
  // food_amount = getFood();
  Serial.print(dispense::food_amount);

  lcd.print(dispense::food_amount);
  lcd.print(" cups");
  lcd.setCursor(0, 1);
  lcd.print("every ");
  lcd.print(dispense::time_interval);
  lcd.print(" minutes");
  dispense::time_interval = dispense::time_interval * (millis() / (1000 * 60));
}

void loop()
{
  IR::recieveIR();

  // unsigned long elapsed_millis = millis();
  // int elapsed_time = elapsed_millis / (1000 * 60);  // Convert milliseconds to minutes
  // Serial.print(elapsed_time);
  // dispenseFood();
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
  float current_time = millis() / (1000.0);
  Serial.print("Current time: ");
  // delay(1000);
  int current_time_int = current_time;
  if ((fmod(current_time, 5.0)) < 1)
  {
    // lcd.clear();
    // lcd.print("Dispensing");
    // while (food_amount <= food_out) {
    // spinFor(180, 1);
    // delay(500);
    // spinFor(180, 1);
    // food_out += 0.25;
    // delay(100);
    //}
    // if (food_amount == food_out){
    // lcd.clear();
    // lcd.print("complete");
    //}
    // delay(2000);
  }
  else
  {
    // lcd.clear();
    // lcd.print("not dispensing");
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

  joystick_x = map(analogRead(X_pin), 0, 1023, -100, 100);
  joystick_y = map(analogRead(Y_pin), 0, 1023, -100, 100);
  if (fabs(joystick_x) < 6)
    joystick_x = 0;
  if (fabs(joystick_y) < 6)
    joystick_y = 0;

  // testing little function
  if (IR::EQ.newPress(true))
  {
    motor.CycleWheel();
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

  LCD::set("time", "");
  LCD::update();
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