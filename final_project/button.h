#pragma once

class button
{
public:
    button(int pin)
    {
        this->pin = pin;
        pinMode(pin, INPUT_PULLUP);
    }
    button() {}

    void off()
    {
        currentState = false;
    }
    void on()
    {
        currentState = true;
    }
    bool newPress()
    {
        currentState = !digitalRead(this->pin);
        bool result = false;
        if (currentState && !lastState)
        {
            result = true;
        }
        lastState = currentState;
        return result;
    }

    bool newPress(bool IR)
    {
        bool result = false;
        if (currentState && !lastState)
        {
            result = true;
        }
        lastState = currentState;
        return result;
    }
    
    bool lastState = false;
    bool currentState = false;
    int pin;
};

extern button limit_switch;
//extern button joystick;