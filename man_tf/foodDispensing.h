#pragma once
#include "lcd.h"
#include "ir.h"
#include "motor.h"
namespace dispense
{

  float time_interval = 0;
  float food_amount = 0;
  int time_unit = 0;
  String time_unit_str[] = {"Sec", "Min", "Hour", "Day"};

  // int time_unit_multiplier[] = {1, 60, 3600, 86400}; // multipliers, corresponds to guy

  String getTimeUnits()
  {

    while (true)
    {
      Serial.print("Time Unit: \n");
      LCD::print("Time Unit: >>|", time_unit_str[time_unit]);

      if (IR::recieveIR())
      {
        if (IR::right.newPress(true))
        {
          time_unit = (time_unit + 1);
          if (time_unit > 3)
            time_unit = 0;
        }
        if (IR::left.newPress(true))
        {
          time_unit--;
          if (time_unit < 0)
            time_unit = 3;
        }
      if (IR::st.newPress(true))
      {
        LCD::set("", "Confirmed");
        LCD::update();
        break;
      }
    }
    delay(20);
    // LCD::update();
  }
  LCD::print("Confirmed", time_unit_str[time_unit]);
  // LCD::print(0, 1, time_unit_str[time_unit], 2);
  return time_unit_str[time_unit];
}

float getTime()
{

  while (true)
  {
    Serial.print("Time: \n");
    LCD::print("Time:      ", "");
    LCD::print(6, 0, time_interval, 2);
    if (IR::recieveIR())
    {
      if (IR::right.newPress(true))
      {
        time_interval += .25;
      }
      if (IR::left.newPress(true) && time_interval > 0)
      {
        time_interval -= .25;
      }
      if (IR::st.newPress(true))
      {
        LCD::set("", "Confirmed");
        LCD::update();
        break;
      }
    }
    delay(20);
  }

  // Display the current time interval
  LCD::print("Confirmed", time_unit_str[time_unit]);
  LCD::print(0, 1, time_interval, 2);
  return time_interval;
}

float getFoodAmount()
{

  while (true)
  {
    Serial.print("Food: \n");
    LCD::print("Food: ", "Cups");
    LCD::print(7, 0, food_amount, 2);
    if (IR::recieveIR())
    {
      if (IR::right.newPress(true))
      {
        food_amount += .25;
      }
      if (IR::left.newPress(true) && food_amount > 0)
      {
        food_amount -= .25;
      }
      if (IR::st.newPress(true))
      {
        break;
      }
    }
    delay(20);
  }

  // Display the current time interval
  LCD::print("Confirmed", "    Cups");
  LCD::print(0, 1, food_amount, 2);
  return food_amount;
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

void dispenseFood()
{
  unsigned long current_time = millis() / (1000);
  delay(1000);
  Serial.print(0 / 1000);
  Serial.print("Current time: ");
  Serial.println(current_time - (0 / 1000));
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

} // namespace dispense
