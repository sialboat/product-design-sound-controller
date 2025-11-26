#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <math.h>
#include <ILI9341_T4.h>
#include <tgx.h>
#include <Adafruit_MCP4728.h>
#include <Adafruit_LSM6DSOX.h>
#include "dsp_filters.h"
#include "ParameterAttribute.h"

#pragma once
typedef struct device_pin {
    String name = "";
    int pin = -1;
} device_pin;

enum class BUTTON_STATE {
    PRESS, HOLD, RELEASE, OFF
};

enum class PARAM_TYPE {
    POT, BUTTON, MISC
};

// base parameter object; pinless
class base_param {

    public:
    virtual bool begin() = 0;
    void debug(bool s, bool d);
    virtual void read() = 0;
    virtual void process() = 0;

    virtual ~base_param() = default;
    base_param() = default;
    String get_name() {return name;}
    
    protected:
    String name;
    void set_name(String n) {name = n;}
    // const char* get_name() {return name.c_str();}
};

// parameter with one pin and one output.
template <typename N>
class single_pin_out_param : public base_param
{
    public:
    single_pin_out_param() = default;
    single_pin_out_param(String n, struct device_pin p) : pin(p) { set_name(n); }
    ~single_pin_out_param();

    bool begin() override;
    void read() override;
    void process() override;
    void debug(bool s, bool d);

    N get_val() {return processed_val; }
    N get_raw_val() {return raw_val; }
    device_pin* get_device_pin() {return &pin;}
    String get_pin()
    {
        String out = pin.name + ": " + pin.pin;
        return out;
    }
    void set_pin_name(String n) {pin.name = n;}
    void set_pin(int p) {pin.pin = p;}
    void set_pin_struct(device_pin p) {pin = p;}
    // attribute class bullshit
    // data behavior
    // user behavior

    protected:
    device_pin pin;
    N processed_val;
    N prev_raw_val;
    N raw_val;
    BUTTON_STATE prev_button_state;
    BUTTON_STATE button_state;
};

template <typename N>
class multi_pin_out_param : public base_param
{
    public:
    multi_pin_out_param() = default;
    multi_pin_out_param(String n, std::initializer_list<device_pin> p_list) : pins(p_list) {set_name(n);}
    ~multi_pin_out_param();

    bool begin() override;
    void read() override;
    void process() override;
    void debug(bool s, bool d);

    std::vector<N>* get_vals() {return &processed_vals; }
    std::vector<N>* get_raw_vals() {return &raw_vals; }
    std::vector<device_pin>* get_device_pins() {return &pins; }
    String get_pin(int i) {return pins.at(i).name + ": " + pins.at(i).pin; }
    String get_pins()
    {
        String out = "";
        for(size_t i = 0; i < pins.size(); i++) {
            out += get_pin(i);
            out += ", ";
            if(i % 5 == 0) out += "\n";
        }
        return out;
    }

    void set_pin(int i, int v) {pins.at(i).pin = v;}
    void set_pin_name(int i, String v) {pins.at(i).name = v;}

    protected:
    std::vector<device_pin> pins;
    std::vector<N> raw_vals;
    std::vector<N> prev_raw_vals;
    std::vector<N> processed_vals;
};

class button : public single_pin_out_param<bool>
{
    public:
    button() : single_pin_out_param<bool>("untitled-button", {"nonexistent-pin", -1})
    {
        
        type = PARAM_TYPE::BUTTON;
    }
    button(String n, device_pin p) : single_pin_out_param<bool>(n, p)
    { 
        
        type = PARAM_TYPE::BUTTON;
    }
    bool begin() override;
    void debug(bool serial, bool display);
    void read() override;
    void process() override;
    PARAM_TYPE get_param_type() {return type;}

    private:
    PARAM_TYPE type;
};

class pot : public single_pin_out_param<int>
{
    public:
    pot() : single_pin_out_param<int>("untitled-pot", {"nonexistent pin", -1})
    {
        type = PARAM_TYPE::POT;
    }
    pot(String n, device_pin p) : single_pin_out_param<int>(n, p) 
    {
        type = PARAM_TYPE::POT;
        filter = moving_average_filter<int>();

    }
    bool begin() override;
    void debug(bool serial, bool display);
    void read() override;
    void process() override;
    PARAM_TYPE get_param_type() {return type;}
    // void set_filter_coeff(float c) {filter.set_coeff(c); }
    // float get_filter_coeff() {return filter.get_coeff(); }

    private:
    PARAM_TYPE type;
    // ADC* adc; // for asynchronous reading; to be implemented later.
    moving_average_filter<int> filter;
};

class joystick
{
    public:
    bool begin(); // this is here for good measure but it will prolly not be used
    void debug(bool s, bool d);
    void read();
    void process();

    joystick() = default;
    ~joystick();

    private:
    String name;
    pot x_pot;
    pot y_pot;
    button select_button;
    int x;
    int y;
    BUTTON_STATE button_state;
};

// class gyroscope : public multi_pin_out_param<float>
// {
//     public:

// };