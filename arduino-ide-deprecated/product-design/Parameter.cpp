#include "Parameter.h"
// #include "ParameterAttribute.h"
// #include "ParameterBehavior.h"

bool button::begin()
{
    // this will be modified upon implementation of 
    // asynchronous calls
    pinMode(pin.pin, INPUT_PULLUP);
    return true;
}

void button::debug(bool s, bool d) 
{
    if(prev_button_state != button_state)
    {
        if(s)
        {
            Serial.println(get_pin());
        }
        if(d)
        {
              
        }
    }
}

void button::read() 
{
    // this will be modified upon implentation of asynchornous calls
    prev_raw_val = raw_val;
    raw_val = !digitalRead(pin.pin); // input pullup
}
// basic processing
void button::process()
{
    prev_button_state = button_state;
    if(!prev_raw_val && !raw_val)
        button_state = BUTTON_STATE::OFF;
    else if(!prev_raw_val && raw_val)
        button_state = BUTTON_STATE::PRESS;
    else if(prev_raw_val && !raw_val)
        button_state = BUTTON_STATE::RELEASE;
    else
        button_state = BUTTON_STATE::HOLD;
}


bool pot::begin() 
{
    // this will be used thoroughly upon implementation of 
    // asynchronous calls, but for now there is nothing 
    // to add here.
    Serial.println("begin");
    filter.set_coeff(0.2f);
    return true;
}
void pot::debug(bool s, bool d) 
{
    if(button_state != prev_button_state)
    {
        if(s)
        {

        }
        if(d)
        {

        }
    }
}
void pot::read() 
{
    prev_raw_val = raw_val;
    raw_val = analogRead(pin.pin);
}
void pot::process() 
{
    processed_val = filter.process(raw_val);
}


// joystick::joystick()
// {

// }
// joystick::~joystick()
// {

// }
bool joystick::begin()
{
    return true;
}

void joystick::debug (bool s, bool d)
{

}

void joystick::read()
{

}
void joystick::process()
{

}
