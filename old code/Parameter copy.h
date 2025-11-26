// temporarily turning this off so arduino doesn't complain to me

// #include <Adafruit_GFX.h>
#include <Encoder.h>
// #include <Adafruit_ILI9341.h>
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

#define WIRE Wire1

#pragma once

// template<int N>
// using num_pins = std::conditional<N == 1, N, std::vector<N>>;

// helper struct for making pins straightfoward.
typedef struct device_pin {
  String name = "";
  int pin = -1;
} device_pin;
// class device_pin
// {
//   public:
//   device_pin(int p, String n) : pin(p), name(n) {}
//   int get_pin() {return pin; }
//   String get_name() {return name; }
//   String to_string() {
//     return name + " : " + pin;
//   }

//   private:
//   int pin;
//   String name;
// };

template<int NUM, typename TYPE>
using vals = typename std::conditional<NUM == 1, TYPE, TYPE[NUM]>::type;
// using vals = typename std::conditional<N == 1, K, std::vector<K>>::type;


enum class PARAM_TYPE {
  BUTTON, POT, JOYSTICK, MISC
};

static String type_to_string(PARAM_TYPE type) {
  switch(type) {
    case BUTTON:
      return "BUTTON";
    break;
    case POT:
      return "POT";
    break;
    case JOYSTICK:
      return "JOYSTICK"
    break;
    case MISC:
      return "MISC";
    break;
    default: // we shouldn't get here.
      return "UNKNOWN";
    break;
  }
}

// base template class that serves as a way to permutate 
template <typename DATATYPE, int PINS, int OUTS>             // N representing the datatype that shit will be stored in
class parameter {                               // K representing how many pins each object will have
  protected:                                    // O representing the number of outputs the parameter will pass
  virtual bool begin() noexcept = 0;            // put the async here thanks; need a callback.
  virtual static void debug() {
    //TODO
  }
  virtual void read() noexcept = 0;
  virtual void process() noexcept = 0;
  // virtual bool add_usr_attr() noexcept = 0;
  // virtual bool add_dat_attr() noexcept = 0;
  // virtual bool rmv_usr_attr() noexcept = 0;
  // virtual bool rmv_dat_attr() noexcept = 0;

  virtual ~parameter() = default;
  parameter(String _name, int _id, moving_average_filter _filter, std::initializer_list<device_pin> _pins) : 
            name(_name), id(_id), filter(_filter)
  {
    
    if(PINS == 1) {
      pins(_pins.name, _pins.pin);
    }
    // K > 1
    if(_pins.size() >= PINS) { // num of pins we get >= num of pins specified in template; pound sand and chop off
      size_t i = 0
      for(; i < std::size(pins); i++) {
        pins[i].pin = _pins.begin()[i].pin;
        pins[i].name = _pins.begin()[i].name;
      }
      Serial.print("number of specified pins are greater than number of allocated pins of param type ");
      Serial.println(type);
      Serial.println("Current uninitialized pins:");
      for(; i < _pins.size(); i++) {
        Serial.print(_pins.begin()[i].name);
        Serial.print(" : ");
        Serial.println(_pins.begin()[i].pin);
      }
    } else if (_pins.size() < PINS) { // num of pins we get < num of pins we asked for; pad with zeros
      size_t i = 0
      for(; i < std::size(pins); i++) {
        pins[i].pin = _pins.begin()[i].pin;
        pins[i].name = _pins.begin()[i].pin;
      }
      for(; i < std::size(pins); i++) {
        pins[i].pin = -1;
        pins[i].name = "unnamed pin";
      }
    }
    
  }

  public:
  String get_name() {return name;}
  int get_id() {return id;}
  vals<PINS, device_pin>* get_pins() { return &pins; }
  vals<OUTS, DATATYPE>* get_raw_val() { return &raw_value; }
  vals<OUTS, DATATYPE>* get_processed_val() { return &processed_value; }
  vals<OUTS, DATATYPE>* get_prev_raw_val() { return &prev_raw_value; } 
  PARAM_TYPE get_param_type() { return type; }
  
  //copied directly from arduino source code, swapped it to floats
  float mapf(float val, float low, float high, float from, float to) {
    return (val - low) * (to - from) / (high - low) + from;
  }
  int mapi(int val, int low, int high, int from, int to) {
    return constrain(map(val, low, high, from, to), from, to);
  }

  protected:
  String name;
  int id;
  vals<PINS, device_pin> pins;
  vals<OUTS, DATATYPE> prev_raw_value;
  vals<OUTS, DATATYPE> raw_value;
  vals<OUTS, DATATYPE> processed_value;
  PARAM_TYPE type;
  // vaks<OUTS, moving_average_filter> filter;
  //attributes
  //data behavior
  //user behavior
};


class pot : public parameter<int, 1, 1>
{
  public:
  pot(String _name, int _id, std::initializer_list<int> _pins, moving_average_filter _filter) 
    : name(_name), id(_id), filter(_filter) { //reminder for silas to initialize the filter inside of the constructor thanks silas
  }

   bool begin() {
    type = POT;
    Serial.println("begin");
    return true;
    // value_reader(pins);
   }

   void debug() noexcept override { // to implement with the display code
    Serial.println("debug");
   }
   void read() noexcept override {
    Serial.println("read");
    // reading = true;
    // read_callback(raw_value, read_callback, this);
   }
   void process() noexcept override {
    Serial.println("process");
    // processed_value = filter.process(raw_value);
   }

   static void read_callback(int val, void* data) {
    // pot *self = static_cast<pot*>(data);
    // self->prev_raw_value = self->raw_value;
    // self->raw_value = val;
    // self->reading = false;
   }
  //  bool add_usr_attr() noexcept override;
  //  bool add_dat_attr() noexcept override;
  //  bool rmv_usr_attr() noexcept override;
  //  bool rmv_dat_attr() noexcept override;

  moving_average_filter* get_filter() {
    return filter;
  } 

  protected:
  ADC* adc;

  private:
  moving_average_filter filter;
  bool reading = false;

};


enum class BUTTON_STATE {
  PRESS, HOLD, RELEASE, OFF
};

class button : public parameter<bool, 1, 1>
{
  public:
  button(String _name, int _id, std::initializer_list<int> _pins) : name(_name), id(_id) {
    type = BUTTON;
    pins = _pins;
  }
  bool begin() noexcept override {
    pinMode(pins, INPUT_PULLUP);
  }
  void debug() noexcept override {
    // display->print()
    Serial.print(name);
    Serial.print(" - raw value ");
    Serial.print(raw_value);
    Serial.print(" - prev raw value - ");
    Serial.print(prev_raw_value);
    Serial.print(" - current state - ");
    Serial.print(button_state);
    Serial.print(" - prev state - ");
    Serial.print(prev_button_state);
    Serial.print(" - pins - ");
    if(O == 1) Serial.println(pins);
    else if (O > 1) {
      for(int i = 0; i < std::size(pins); i++) {
        Serial.println(pins[i]);
      }
    }
  }
  void read() noexcept override {
    prev_raw_value = raw_value;
    raw_value = !digitalRead(pins); //input pullup
  }
  void process() noexcept override {
    prev_button_state = button_state;
    
    if(!prev_raw_value && !raw_value)
      button_state = BUTTON_STATE::OFF;
    else if(!prev_raw_value && raw_value)
      button_state = BUTTON_STATE::PRESS;
    else if(prev_raw_value && !raw_value)
      button_state = BUTTON_STATE::RELEASE;
    else 
      button_state = BUTTON_STATE::HOLD;
  }
  // bool add_usr_attr() noexcept override;
  // bool add_dat_attr() noexcept override;
  // bool rmv_usr_attr() noexcept override;
  // bool rmv_dat_attr() noexcept override;

  private:
  BUTTON_STATE button_state = BUTTON_STATE::OFF;
  BUTTON_STATE prev_button_state = BUTTON_STATE::HOLD;
};

//this one will literally just be a wrapper lmao
class joystick // : public pot, button
{
  public:
  joystick(pot&& _x, pot&& _y, button&& _sel) : x(std::move(_x)), y(std::move(_y)), sel(std::move(_sel)) {
    type = JOYSTICK;
  }

  private:
  pot x;
  pot y;
  button sel;
};

enum class GYROSCOPE_VALS {
  ACCEL_X = 0, ACCEL_Y = 1, ACCEL_Z = 2, GYRO_X = 3, GYRO_y = 4, GYRO_Z = 5, TEMP = 6
};

class gyroscope : public parameter<float, 2, 7>
{
  public:

  namespace GYRO_INDEX
  {
    const int ACCELX = 0;
    const int ACCELY = 1;
    const int ACCELZ = 2;
    const int GYROX = 3;
    const int GYROY = 4;
    const int GYROZ = 5;
    const int TEMP = 6;
    
  }

  gyroscope(Adafruit_LSM6DSOX, String _name, int _id, std::initializer_list<float> _coeffs) : 
  name(_name), id(_id)
  {

    type = MISC;
    accel_x = accel_y = accel_z =
    gyro_x  = gyro_y. = gyro_z  =
    temperature = 0.0f;

    for(float f : _coeffs.v)
      filters.push_back(moving_average_filter<float>(f));
  }

  bool begin() noexcept override {
    if(!gyro.begin_I2C(0x60, &Wire)) {
      Serial.println("Failed to find LSM6DSOX Gyroscope!");
      return false;
    }
    Serial.println("We found the LSM6DSOX Gyroscope! Setting default range");
    gyro.setGyroRange(LSM6DS_GYRO_RANGE_500_DPS);
    return true;
  }

  void read() noexcept override {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    raw_accel_x = acel.acceleration.x;
    raw_accel_y = acel.acceleration.y;
    raw_accel_z = acel.acceleration.z;
    raw_gyro_x = gyro.gyro.x;
    raw_gyro_y = gyro.gyro.y;
    raw_gyro_z = gyro.gyro.z;
    raw_temperature = temp.temperature;
  }

  void process() noexcept override {
    accel_x = filters.at(0).process(raw_accel_x);
    accel_y = filters.at(1).process(raw_accel_y);
    accel_z = filters.at(2).process(raw_accel_z);
    gyro_x = filters.at(3).process(raw_gyro_x);
    gyro_y = filters.at(4).process(raw_gyro_y);
    gyro_z = filters.at(5).process(raw_gyro_z);
    temperature = filters.at(6).process(raw_temperature);
  }

  void debug() noexcept override {
    String accel = "Accel - X: " + accel_x + "; Y: " + accel_y + "; Z: " + accel_z;
    String gyro = "Gyro - X: " + gyro_x + "; Y: " + gyro_y + "; Z: " + gyro_z;
    String temp = "Temp: - " + temperature;
    String raw_accel = "Accel - X: " + raw_accel_x + "; Y: " + raw_accel_y + "; Z: " + raw_accel_z;
    String raw_gyro = "Gyro - X: " + raw_gyro_x + "; Y: " + raw_gyro_y + "; Z: " + raw_gyro_z;
    String raw_temp = "Temp: - " + raw_temperature;
    Serial.println(name);

    Serial.println(" --- raw values --- ");
    Serial.println(raw_accel);
    Serial.println(raw_gyro);
    Serial.println(raw_temp);

    Serial.println(" --- processed values --- ");
    Serial.println(accel);
    Serial.println(gyro);
    Serial.println(temp);

    Serial.println(" --- pins --- ");
    Serial.println();

  }

  void set_coeff(int index, int )

  private:
  Adafruit_LSM6DSOX gyro;
  std::vector<moving_average_filter> filters;
  
  // filter indexes are based on the ordering of these values.
  float raw_accel_x, raw_accel_y, raw_accel_z,
        raw_gyro_x,  raw_gyro_y,  raw_gyro_z,
        raw_temperature;

  float accel_x, accel_y, accel_z,
        gyro_x,  gyro_y,  gyro_z,
        temperature;
};

// enum class DAC_VALS {
//   CHANNEL_1 = 0, CHANNEL_2 = 1, CHANNEL_3 = 2, CHANNEL_4 = 3
// };
// this technically isn't a paremeter.
// class dac
// {
//   public:
//   dac(String _name, int _id, std::initializer_list<int> _pins, std::initializer_list<float> coeffs) :
//   name(_name), id(_id)
//   {
//     param_type = MISC;
//   }
// };
//This serves to exist so we have a framework for how most parameter objects will exist
// template <typename N>
// class parameter {
//   protected:
//   virtual bool begin() noexcept = 0;                            // initialize parameter
//   virtual void update() noexcept = 0;                           // read and update raw values; light processing
//   virtual void process() noexcept = 0;                          // process raw values based on listed attributes
//   N get_raw_value() {return raw_value; }                // returns values from update()
//   N get_processed_value() {return processed_value; }    // returns processed values from process()

//   virtual ~parameter() = default;

//   private:
//   String name;
//   PARAM_TYPE param_type;
//   N raw_value, processed_value;
// };