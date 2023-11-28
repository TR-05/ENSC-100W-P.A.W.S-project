#pragma once
#include "lcd.h"
#include "ir.h"
namespace dispense {

float time_interval = 0;
float food_amount = 0;
int getTime()
{

  while (true)
  {
    LCD::print("Time: ", "Minutes");
    LCD::print(7, 0, time_interval);
    if (IR::recieveIR())
    {
      if (IR::right.newPress(true))
      {
        time_interval++;
      }
      if (IR::right.newPress(true) && time_interval > 0)
      {
        time_interval--;
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
  LCD::print("Time: ", " minutes");
  LCD::print(7, 0, time_interval);
  return time_interval;
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
  Serial.print(0/1000);
  Serial.print("Current time: ");
  Serial.println(current_time - (0/1000));
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