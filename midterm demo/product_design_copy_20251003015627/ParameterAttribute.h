// // temporarily turning this off so arduino doesn't complain to me

// #pragma once

// #include <map>
// #include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// // #include <Adafruit_GFX.h>
// #include <math.h>
// #include <ILI9341_T4.h>
// #include <tgx.h>
// #include <Encoder.h>
// // #include <Adafruit_ILI9341.h>
// #include <Adafruit_NeoPixel.h>
// #include <Adafruit_MCP4728.h>
// #include <Adafruit_LSM6DSOX.h>

// enum class ATTR_TYPE {
//   USER, DATA, GUI
// };
// // // template<int N, typename K>
// // // using vals = std::conditional<N == 1, N, >

// // //small class outlining an attribute class
// // //single data type
// // template <ATTR_TYPE T>
// class attribute_base {
//   protected:
//   virtual void process() noexcept = 0;

//   attribute();
//   virtual ~attribute() = default;

//   public:
//   ATTR_TYPE get_attr_type() {return attr_type;}

//   private: 
//   ATTR_TYPE attr_type;
// };

// template <typename TYPE_IN, typename TYPE_OUT>
// class attribute : public attribute_base
// {
//   protected:
//   virtual void process(TYPE_IN* val, TYPE_OUT* val) noexcept override;

// };

// template <typename TYPE_IN, typename TYPE_OUT>
// class data_attr : public attribute



// // class data_attr : public attribute

// // template <typename N>
// // class user_attr : public attr