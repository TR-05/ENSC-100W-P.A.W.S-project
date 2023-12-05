#pragma once
#include "motor.h"

namespace time
{
    unsigned long long set_up_time_ms = 0;
    unsigned long long time_ms = 0;
    double time_s = 0;
    double time_m = 0;
    double time_h = 0;
    double time_d = 0;

    void update()
    {
        time_ms = millis() - set_up_time_ms;
        time_s = time_ms / 1000.0;
        time_m = time_s / 60.0;
        time_h = time_m / 60.0;
        time_d = time_h / 24.0;
    }
     
     //float dispense_interval = time_interval/time_unit_multiplier[time_unit];
    void checkForDispense(float minutes, int speed, float cups)
    {
        float interval_ms = minutes * 60 * 1000;
        int cycles = (cups * 4.0);

        if ((fmod(time_ms, interval_ms)) < 1000)
        {
            for (int i = 0; i < cycles; i++)
            {
                
                motor.spinFor(195, 255, 1);
                delay(300);
                motor.CycleWheel(speed);
                delay(500);
            }
        }
    }

}
