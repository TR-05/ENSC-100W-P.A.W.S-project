#pragma once
#include "ports.h"
#include "lcd.h"
#include <string.h>

class MOTOR
{
public:
    const int ENABLE = 5;
    const int DIRA = 3;
    const int DIRB = 4;
    const float motorMaxRPM = 6372;
    const float outputGearRPM = 34.3;
    const float ratio = outputGearRPM / motorMaxRPM;

    void initialize()
    {
        pinMode(ENABLE, OUTPUT);
        pinMode(DIRA, OUTPUT);
        pinMode(DIRB, OUTPUT);
    }

    /*
     * @brief Stops the motor
     */
    void spin(float speed, bool backwards = false)
    {
        if (speed == 0)
        {
            digitalWrite(DIRA, LOW); // fast stop
            digitalWrite(DIRB, LOW); // fast stop
        }
        analogWrite(ENABLE, speed); // set velocity
        if (backwards)
        {
            digitalWrite(DIRA, HIGH);
            digitalWrite(DIRB, LOW);
        }
        else
        {
            digitalWrite(DIRA, LOW);
            digitalWrite(DIRB, HIGH);
        }
    }

    void spinFor(float deg, float speed, int dir)
    {

        spin(0);
        delay(200);
        if (dir == 1)
        {
            spin(speed, false);
        }
        if (dir == -1)
        {
            spin(speed, true);
        }
        if (dir != 1 && dir != -1)
        {
            return;
        }
        int timeToSpinFor = rotationTimeMs(deg, speed);
        int motorStartMillis = millis();
        int loopCounter = 0;

        Serial.print("Time to spin for: ");
        Serial.print(timeToSpinFor);
        Serial.print("\n startMillis");
        Serial.print(motorStartMillis);
        Serial.print("\n millis");
        Serial.print(millis());

        Serial.print("\n loop condition: ");
        Serial.print(millis() - motorStartMillis < timeToSpinFor);

        Serial.print("\n\n\n");

        delay(timeToSpinFor);
        spin(255, true);
        delay(70);
        spin(0);
        Serial.print("\ndone");
    }

    void CycleWheel(int speed)
    {
        spin(speed, false);

        bool limit_switch_pressed = !digitalRead(limit_switch_port);
        ;
        bool last_limit_switch_pressed = limit_switch_pressed;
        while (true)
        {
            limit_switch_pressed = !digitalRead(limit_switch_port);
            if (limit_switch_pressed != last_limit_switch_pressed && limit_switch_pressed)
            {
                LCD::set("Done", "1");
                break;
            }
            else
            {
                LCD::set("Spinning", "0");
            }
            last_limit_switch_pressed = limit_switch_pressed;
            delay(10);
            LCD::update();
        }

        spin(255, true);
        delay(70);
        spin(0);
        LCD::set("Stopped", "1");
        LCD::update();
    }
    int rotationTimeMs(float degrees, float speed)
    {
        float targetRotations = degrees / 360;
        float rotations_of_motor = targetRotations / ratio;
        float desiredRPM = map(speed, 0, 255, 0, motorMaxRPM);
        float output = rotations_of_motor / desiredRPM;
        output *= 60;
        output *= 1000;
        Serial.print("\nTime MS: ");
        Serial.print(output);
        Serial.print("\n");
        return int(output);
    }
};
MOTOR motor;
