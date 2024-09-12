#pragma once
#include "lcd.h"
#include "ir.h"
#include "motor.h"
#include "time.h"
namespace dispense
{

  float time_interval = 0;
  float food_amount = 0;
  int time_unit = 0;
  String time_unit_str[] = {"Sec", "Min", "Hour", "Day"};

  float time_unit_multiplier[] = {1000, 60000, 3600000,   86400000}; // multipliers, corresponds to guy
  /*
  1/1000 = ms to sec
  millis()/60000 = min
  millis()/3600000 = hour
  millis()/86400000 = day
  */

  String getTimeUnits()
  {

    while (true)
    {
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
    LCD::print("Time Interval:  >>|  ", "");
    LCD::print(6, 0, time_interval, 2);
    if (IR::recieveIR())
    {
      if (IR::right.newPress(true))
      {
        time_interval += .50;
      }
      if (IR::left.newPress(true) && time_interval > 0)
      {
        time_interval -= .50;
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
    LCD::print("Food: >>|", "Cups");
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

void dispenseFood()
{
  float dispense_interval = time_interval*time_unit_multiplier[time_unit];
  int cycles = (food_amount * 4.0);
  if ((fmod(time::time_ms, dispense_interval)) < 1000) 
  {
    for (int i = 0; i < cycles; i++)
    {
      motor.spinFor(195, 255, 1);
      delay(300);
      motor.CycleWheel(255);
      delay(500);
    }
  }
  }



} // namespace dispense
