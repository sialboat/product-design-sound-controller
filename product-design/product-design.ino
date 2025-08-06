#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <math.h>
#include <Encoder.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MCP4728.h>
#include <Adafruit_LSM6DSOX.h>

#include "dsp_filters.h"

typedef struct Button {
  const int pin;
  bool state = false;
} BUTTON;

typedef struct Potentiometer {
  const int pin;
  int value = 0;
} POT;

namespace PINOUTS
{
  namespace TFT_DISPLAY
  {
    const int DC = 33;
    const int CS = 36;
    const int SCK = 13;
    const int MOSI = 11;
  };

  namespace MCP4728_DAC
  {
    const int SCL = 19;
    const int SDA = 18;
  };

  namespace LSM6DSOX_GYRO
  {
    const int SCL = 16;
    const int SDA = 17;
  };

  namespace JOYSTICK_L
  {
    const int SELECT = 31;
    const int YOUT = A10;
    const int XOUT = A11;
  };

  namespace JOYSTICK_R
  {
    const int SELECT = 32;
    const int YOUT = A12;
    const int XOUT = A13;
  };

  namespace DPAD_BUTTONS
  {
    const int UP = 0;
    const int DOWN = 1;
    const int LEFT = 2;
    const int RIGHT = 3;
  };

  namespace XYAB_BUTTONS
  {
    const int X = 4;
    const int Y = 5;
    const int A = 6;
    const int B = 7;
  };

  namespace TRIGGERS
  {
    const int LT = 41;
    const int RT = 40;
  };

  namespace MISC_BUTTONS
  {
    const int HOME = 8;
    const int MENU = 9;
  };
  
  //TODO
  namespace NEOPIXEL
  {

  };

  namespace OLDSTICK
  {
    const int LR = A8;
    const int UD = A9;
  };

};

namespace FILTERS
{

  namespace JOYSTICK
  {
    float filter_coef = 0.2f;
    moving_average_filter left_joystick_up_down(filter_coef);
    moving_average_filter left_joystick_left_right(filter_coef);
    moving_average_filter right_joystick_up_down(filter_coef);
    moving_average_filter right_joystick_left_right(filter_coef);
    moving_average_filter filters[4] = {left_joystick_up_down, left_joystick_left_right, right_joystick_up_down, right_joystick_left_right};
  }; 

  namespace TRIGGERS
  {
    float filter_coef = 0.2f;
    moving_average_filter left_trigger(filter_coef);
    moving_average_filter right_trigger(filter_coef);
  };

};

BUTTON x_button {PINOUTS::XYAB_BUTTONS::X};
BUTTON y_button {PINOUTS::XYAB_BUTTONS::Y};
BUTTON a_button {PINOUTS::XYAB_BUTTONS::A};
BUTTON b_button {PINOUTS::XYAB_BUTTONS::B};

BUTTON dpad_up_button {PINOUTS::DPAD_BUTTONS::UP};
BUTTON dpad_down_button {PINOUTS::DPAD_BUTTONS::DOWN};
BUTTON dpad_left_button {PINOUTS::DPAD_BUTTONS::LEFT};
BUTTON dpad_right_button {PINOUTS::DPAD_BUTTONS::RIGHT};

BUTTON misc_home_button {PINOUTS::MISC_BUTTONS::HOME};
BUTTON misc_menu_button {PINOUTS::MISC_BUTTONS::MENU};

POT left_joystick_x {PINOUTS::JOYSTICK_L::XOUT};
POT left_joystick_y {PINOUTS::JOYSTICK_L::YOUT};
BUTTON left_joystick_select {PINOUTS::JOYSTICK_L::SELECT};

POT right_joystick_x {PINOUTS::JOYSTICK_R::XOUT};
POT right_joystick_y {PINOUTS::JOYSTICK_R::YOUT};
BUTTON right_joystick_select {PINOUTS::JOYSTICK_R::SELECT};

POT left_trigger {PINOUTS::TRIGGERS::LT};
POT right_trigger {PINOUTS::TRIGGERS::RT};

POT oldstick_lr {PINOUTS::OLDSTICK::LR};
POT oldstick_ud {PINOUTS::OLDSTICK::UD};

#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240

Adafruit_ILI9341 display = Adafruit_ILI9341(PINOUTS::TFT_DISPLAY::CS, PINOUTS::TFT_DISPLAY::DC);
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel();
Adafruit_MCP4728 mcp_dac;
Adafruit_LSM6DSOX sox_gyro;

unsigned long lastFrame = 0;
int frameRate = 30;

void setup() {
  Serial.begin(9600);

  display.begin();

  // if(!display.begin()) {
  //   Serial.println("Failed to find ILI9341 display!");
  //   while(true) {
  //     delay(10);
  //   }
  // }z

  if(!mcp_dac.begin()) {
    Serial.println("Failed to find MCP4728 chip");
    while(true) {
      delay(10);
    }
  }

  display.fillScreen(ILI9341_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);
  display.setRotation(4);
  display.println("good morning does the text works");
}

void loop() {

  left_joystick_x.value = FILTERS::JOYSTICK::left_joystick_up_down.process(analogRead(PINOUTS::JOYSTICK_L::XOUT));
  left_joystick_y.value = FILTERS::JOYSTICK::left_joystick_left_right.process(analogRead(PINOUTS::JOYSTICK_L::YOUT));

  right_joystick_x.value = FILTERS::JOYSTICK::right_joystick_up_down.process(analogRead(PINOUTS::JOYSTICK_R::XOUT));
  right_joystick_y.value = FILTERS::JOYSTICK::right_joystick_left_right.process(analogRead(PINOUTS::JOYSTICK_R::YOUT));

  left_trigger.value = FILTERS::TRIGGERS::left_trigger.process(analogRead(PINOUTS::TRIGGERS::LT));
  right_trigger.value = FILTERS::TRIGGERS::right_trigger.process(analogRead(PINOUTS::TRIGGERS::RT));

  if(millis() > lastFrame + frameRate) {
    lastFrame = millis();
    DEBUG_clear_text(left_joystick_x.value, 100, 100);
    DEBUG_clear_text(right_joystick_x.value, 175, 100);
    DEBUG_display_text(left_joystick_x.value, ILI9341_WHITE, ILI9341_BLACK, 100, 100);    
    DEBUG_display_text(right_joystick_x.value, ILI9341_WHITE, ILI9341_BLACK, 175, 100);

    DEBUG_clear_text(left_joystick_y.value, 100, 150);
    DEBUG_clear_text(right_joystick_y.value, 175, 150);
    DEBUG_display_text(left_joystick_y.value, ILI9341_WHITE, ILI9341_BLACK, 100, 150);
    DEBUG_display_text(right_joystick_y.value, ILI9341_WHITE, ILI9341_BLACK, 175, 150);

    DEBUG_clear_text(left_trigger.value, 100, 200);
    DEBUG_clear_text(right_trigger.value, 175, 200);

    DEBUG_display_text(left_trigger.value, ILI9341_WHITE, ILI9341_BLACK, 100, 200);
    DEBUG_display_text(right_trigger.value, ILI9341_WHITE, ILI9341_BLACK, 175, 200);

    Serial.print(analogRead(PINOUTS::JOYSTICK_L::XOUT));
    Serial.print(" ");
    Serial.println(analogRead(PINOUTS::JOYSTICK_L::YOUT));

    // Serial.print(left_trigger.value);
    // Serial.print(" ");
    // Serial.print(right_trigger.value);
    // Serial.print(" ");
    // Serial.print(joyX_filtered);
    // Serial.print(" ");
    // Serial.print(joyY_filtered);
    // Serial.print(" ");
    // Serial.print(joyXR_filtered);
    // Serial.print(" ");
    // Serial.print(joyYR_filtered);
    // Serial.print(" ");
    // Serial.print(analogRead(PINOUTS::OLDSTICK::LR));
    // Serial.print(" ");
    // Serial.print(analogRead(PINOUTS::OLDSTICK::UD));
    // Serial.println(" ");
    
  }

}

// int smooth_signal(int input_pin, int threshold) {
//   int prev = input;
//   int current = 0;
//   Serial.print("DEBUG prev - ");
//   Serial.println(prev);

//   if(current != 0) {
//     if(input >= prev + threshold || input >= prev - threshold) {
//       input++;
//     }
//   }

//   current = analogRead(input_pin);

//   return input;
// }

/**
 * Helper function that combines the map() and constrain() function into one, guaranteeing that a value from
 * the PIN potentiometer's VALUE is constrained from the values "LOW" to the values "HIGH."
 * @param pin the potentiometer's pin value
 * @param value the variable storing the potentiometer's value
 * @param from the lower bound of value's current range
 * @param to the upper bound of value's upper range
 * @param low the lower bound of the value's target range
 * @param high the upper bound of the value's target range
 * @return a constrained float or int value between the values LOW and HIGH
 */

float constrainValues (int pin, float value, float from, float to, float low, float high) {
  value = map(analogRead(pin), from, to, low, high);
  value = constrain(value, low, high);
  return value;
}
int constrainValues(int pin, int value, int from, int to, int low, int high) {
  value = map(analogRead(pin), from, to, low, high);
  value = constrain(value, low, high);
  return value;
}

void DEBUG_display_text(String value, int text_color, int bg_color, int x, int y) {
  display.setCursor(x, y);
  display.setTextColor(text_color, bg_color);
  display.print(value);
}
void DEBUG_clear_text(String value, int x, int y) {
  display.setCursor(x, y);
  display.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
  display.print(value);

  // for(int i = 0; i < 4; i++) {
  //   display.print(0xDA);
  // }
}
