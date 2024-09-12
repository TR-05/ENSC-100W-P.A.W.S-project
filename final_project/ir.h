#pragma once
#include "IRremote.h"
#include "ports.h"
#include "button.h"
namespace IR
{
    IRrecv irrecv(receiver); // create instance of 'irrecv'
    decode_results results;  // create instance of 'decode_results'
    button volUp;
    button volDown;
    button _0;
    button _1;
    button _2;
    button _3;
    button _4;
    button _5;
    button _6;
    button _7;
    button _8;
    button _9;
    button power;
    button up;
    button down;
    button left;
    button right;
    button st;
    button func;
    button play;
    button EQ;

    void intialize()
    {
        irrecv.enableIRIn(); // Start the receiver
    }


    bool recieveIR()
    {
        volUp.off();
        volUp.off();
        _0.off();
        _1.off();
        _2.off();
        _3.off();
        _4.off();
        _5.off();
        _6.off();
        _7.off();
        _8.off();
        _9.off();
        power.off();
        up.off();
        down.off();
        left.off();
        right.off();
        st.off();
        func.off();
        play.off();
        EQ.off();
        bool output = false;
        if (irrecv.decode(&results)) // have we received an IR signal?
        {
            output = true;
            // Serial.print("New Press: ");
            // Serial.print(results.value, HEX);
            // Serial.println();
            switch (results.value)
            {
                // case 0xFFFFFFFF:
                //   break;
            case 0xFFA857: // VOL- button pressed
                volDown.on();
                break;
            case 0xFF629D: // VOL+ button pressed
                volUp.on();
                break;
            case 0xFF6897: // 0
                _0.on();
                break;
            case 0xFFA25D: // Power
                power.on();
                break;
            case 0xFFE21D: // FUNC/STOP
                func.on();
                break;
            case 0xFF22DD: // left arrow
                left.on();
                break;
            case 0xFF02FD: // Pause/Play
                play.on();
                break;
            case 0xFFC23D: // Right Arrow
                right.on();
                break;
            case 0xFFE01F: // Down Arrow
                down.on();
                break;
            case 0xFF906F: // Up Arrow
                up.on();
                break;
            case 0xFF9867: // button_EQ
                EQ.on();
                break;
            case 0xFFB04F: // ST/REPT
                st.on();
                break;
            case 0xFF30CF: // 1
                _1.on();
                break;
            case 0xFF18E7: // 2
                _2.on();
                break;
            case 0xFF7A85: // 3
                _3.on();
                break;
            case 0xFF10EF: // 4
                _4.on();
                break;
            case 0xFF38C7: // 5
                _5.on();
                break;
            case 0xFF5AA5: // 6
                _6.on();
                break;
            case 0xFF42BD: // 7
                _7.on();
                break;
            case 0xFF4AB5: // 8
                _8.on();
                break;
            case 0xFF52AD: // 9
                _9.on();
                break;
            }

            irrecv.resume(); // receive the next value
        }
        return output;
    }

}