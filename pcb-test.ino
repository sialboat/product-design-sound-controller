// pcb-test.ino

#include <NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <math.h>
#include <Encoder.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MCP4728.h>
#include <Adafruit_LSM6DSOX.h>

namespace PINOUTS
{
  namespace TFT_DISPLAY
  {
    const int CS = 10;
    const int MOSI = 11;
    const int MISO = 12;
    const int SCK = 13;
    const int RST = 33;
    const int DC = 34;
  };

  namespace MCP4728_DAC
  {
    const int SCL = 19;
    const int SDA = 18;
  };

  namespace LSM6DSOX_GYRO
  {
    // REMINDER FOR SILAS TO SWITCH THE PINS ON THE FUCKING HARDWARE THANK YOU FUTURE SILAS LOVE YOU FROM SLEEP DEPRIVED PAST SILAS
    // Supposedly this works because I2C is amazing and beautiful and something I don't understand yet but also amazing and beautiful.
    const int SCL = 19;
    const int SDA = 18;
    // const int SCL = 16; 
    // const int SDA = 17;
  };

  namespace JOYSTICK_L
  {
    const int SELECT = 2;
    const int XOUT = A6;
    const int YOUT = A7;
  };

  namespace JOYSTICK_R
  {
    const int SELECT = 3;
    const int XOUT = A8;
    const int YOUT = A9;
  };

  namespace DPAD_BUTTONS
  {
    const int UP = 32;
    const int DOWN = 31;
    const int LEFT = 30;
    const int RIGHT = 29;
  };

  namespace XYAB_BUTTONS
  {
    const int X = 28;
    const int Y = 27;
    const int A = 26;
    const int B = 25;
  };

  namespace TRIGGERS
  {
    const int LT = 14;
    const int RT = 15;
    const int LB = 7;    //IS A BUTTON
    const int RB = 6;    //IS A BUTTON
  };

  namespace MISC_BUTTONS
  {
    const int MSC1 = 9;
    const int MSC2 = 8;
  };
  
  //TODO
  namespace NEOPIXEL
  {

  };

  namespace SPEAKER
  {
    
  };
  //namespace for VIBRATION THING TOO
  
  namespace OLDSTICK
  {
    const int LR = A8;
    const int UD = A9;
  };


};


void setup()
{

}

void loop()
{

}

