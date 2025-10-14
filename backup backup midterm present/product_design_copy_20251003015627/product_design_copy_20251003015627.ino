#include <SPI.h>
#include <Wire.h>
#include <math.h>
#include <Encoder.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MCP4728.h>
#include <Adafruit_LSM6DSOX.h>
// #include <ILI9341_T4.h>
#include <tgx.h>
#include <Adafruit_GFX.h>

// #include <Adafruit_GFX_Buffer.h>
// #include <XInput.h>
// #include <ResponsiveAnalogRead.h>
#include <Keyboard.h>
#include <usb_midi.h>
// #include <ADC.h>
// #include <analogReadAsync.h>
// #include <tgx_font.h>

#include "dsp_filters.h"
// #include "debug.h"

#include <font_tgx_OpenSans_Bold.h>
#include "font_tgx_OpenSans.h"
#include "font_tgx_Arial_Bold.h"

/*
  Constants with simple names representing pinouts
*/
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

typedef struct HADES_DEMO
{

};

typedef struct MINECRAFT_DEMO
{

};

/*
Enums to handle the different ways peripherals can be configured for transmitting Non-HID related Data.
*/

// Button states to streamline button logic (Steve I wrote my own sort of half-baked implementation of your
// awesome button please do not yell at me)
enum class BUTTON_STATE {
 OFF,       // Button is not pressed. (!state && !prev_state)
 PRESS,     // Button is pressed (state && !prev_state)
 HOLD,      // Button is held (state && prev_state)
 RELEASE    // Button is released (!state && prev_state)
};

// Button types to configure button behavior for interpreting button behavior
enum class BUTTON_TYPE {
  TAP_HOLD,     // Default button logic (trigger / gate for tap and hold, also tracks release)
  TOGGLE,       // On/Off button handler (will remain on / off until it is toggled)
  PROBS,        // Manually determine the probability the button will return an "A" or "B" (chainable for future implementations )upon button press
  SWITCH        // Cycles (increments, decrements, randomly generates values, etc.) based on Switch behavior.
};

//Switch behaviors for how the button will cycle between different values. Hopefully these will be chainable
//so the user can add as many behaviors as they would like
enum class SWITCH_TYPE {
  BIAS,     // Offset cycle behavior by a constant factor
  SCALE,    // Offset cycle behavior by a linear function
  WARP,     // Offset cycle behavior by a mathematical function
  PROBS,    // Adjust the 
  CYCLE     // Determines the pattern by which the values will cycle (analagous to arpeggio patterns)
};

// Behaviors for how Potentiometers will be sent using MIDI.
enum class POT_BEHAVIOR {
  X_OP_Y, // Data will be displayed as X <math operation goes here> Y
  X_OR_Y, // Data will be sent in two separate channels, one for X, one for Y.
  POLAR,  // Data will be displayed as a Polar Coordinate / in Radians
  //other stuff here will be stuff for signal processing, Pot controls some sort of values
  //ie input for Hilbert Transform, paramters for Lorenz attractors, etc.
};

/*
  Structs encapsulating data related to certain peripherals.
  TO BE DEPRECATED SOON
*/

typedef struct Button {
  const int pin;
  String name;
  int key_press;

  bool state = false;
  bool prev_state = true;   //so we can initially debug and display the thing

  BUTTON_STATE button_state = BUTTON_STATE::OFF;
  BUTTON_STATE prev_button_state = BUTTON_STATE::HOLD;
  bool debug = false;
} BUTTON;

typedef struct Potentiometer {
  const int pin;
  moving_average_filter* filter;
  String name;
  // char key_press;
  int value = 0;
  int prev_value = 0;

  bool debug = false;
} POT;

typedef struct Joystick {
  POT x_pot;
  POT y_pot;
  BUTTON select_button;
} JOYSTICK;

//Structs encapsulating DAC and Gyroscope values
// TO BE DEPRECATED SOON
typedef struct DAC {
  Adafruit_MCP4728* dac;
  int chan1_val = 0;
  int chan2_val = 0;
  int chan3_val = 0;
  int chan4_val = 0;
  int dac_vals[4] = {0};
  int prev_dac_vals[4] = {0};
} DAC;

typedef struct Gyroscope {
  Adafruit_LSM6DSOX* gyroscope;
  float accel_x = 0.0f;         //meters per second squared
  float accel_y = 0.0f;         //meters per second squared
  float accel_z = 0.0f;         //meters per second squared
  float gyro_x = 0.0f;          //radians
  float gyro_y = 0.0f;          //radians
  float gyro_z = 0.0f;          //radians
  float temperature = 0.0f;     //degrees celcius
  float gyro_vals[7] = {0.0f};
  float prev_gyro_vals[7] = {0.0f};
} GYRO;

#define WIRE Wire1
#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240
/*
  pins to avoid for analog read (from https://forum.pjrc.com/index.php?threads%2Fteensy-4-1-adc-channels.72373%2F)
  255,	// 10/A10 AD_B0_12 - only on ADC1, 1 - can't use for audio
	255,	// 11/A11 AD_B0_13 - only on ADC1, 2 - can't use for audio
  ...
	255,    // 24/A10 AD_B0_12 - only on ADC1, 1 - can't use for audio
	255,    // 25/A11 AD_B0_13 - only on ADC1, 2 - can't use for audio
	3,      // 26/A12 AD_B1_14 - only on ADC2, do not use analogRead()
	4,      // 27/A13 AD_B1_15 - only on ADC2, do not use analogRead()
    #ifdef ARDUINO_TEENSY41
  ...
	1,      // 38/A14 AD_B1_12 - only on ADC2, do not use analogRead()
	2,      // 39/A15 AD_B1_13 - only on ADC2, do not use analogRead()

*/

Adafruit_ILI9341 display = Adafruit_ILI9341(PINOUTS::TFT_DISPLAY::CS, PINOUTS::TFT_DISPLAY::DC);

Adafruit_NeoPixel neopixel = Adafruit_NeoPixel();
Adafruit_MCP4728 mcp_dac;
Adafruit_LSM6DSOX sox_gyro;

float joystick_filter_coef = 0.9f;
moving_average_filter ljud_filter(joystick_filter_coef, 0);
moving_average_filter ljlr_filter(joystick_filter_coef, 0);
moving_average_filter rjud_filter(joystick_filter_coef, 0);
moving_average_filter rjlr_filter(joystick_filter_coef, 0);
moving_average_filter* joystick_filters[4] = {&ljud_filter, &ljlr_filter, &rjud_filter, &rjlr_filter};

float triggers_filter_coef = 0.9f;
moving_average_filter lt_filter(triggers_filter_coef, 2);
moving_average_filter rt_filter(triggers_filter_coef, 2);
moving_average_filter* trigger_filters[2] = {&lt_filter, &rt_filter};

BUTTON x_button = {PINOUTS::XYAB_BUTTONS::X, "X", KEY_X};
BUTTON y_button = {PINOUTS::XYAB_BUTTONS::Y, "Y", KEY_Y};
BUTTON a_button = {PINOUTS::XYAB_BUTTONS::A, "A", KEY_A};
BUTTON b_button = {PINOUTS::XYAB_BUTTONS::B, "B", KEY_B};

BUTTON dpad_up_button = {PINOUTS::DPAD_BUTTONS::UP, "U", KEY_UP};
BUTTON dpad_down_button = {PINOUTS::DPAD_BUTTONS::DOWN, "D", KEY_DOWN};
BUTTON dpad_left_button = {PINOUTS::DPAD_BUTTONS::LEFT, "L", KEY_LEFT};
BUTTON dpad_right_button = {PINOUTS::DPAD_BUTTONS::RIGHT, "R", KEY_RIGHT};  

BUTTON misc_home_button = {PINOUTS::MISC_BUTTONS::MSC1, "MS1"};
BUTTON misc_menu_button = {PINOUTS::MISC_BUTTONS::MSC2, "MS2"};

POT left_joystick_x = {PINOUTS::JOYSTICK_L::XOUT, &ljud_filter, "L-X"};
POT left_joystick_y = {PINOUTS::JOYSTICK_L::YOUT, &ljlr_filter, "L-Y"};
BUTTON left_joystick_select = {PINOUTS::JOYSTICK_L::SELECT, "L-SEL"};

POT right_joystick_x = {PINOUTS::JOYSTICK_R::XOUT, &rjud_filter, "R-X"};
POT right_joystick_y = {PINOUTS::JOYSTICK_R::YOUT, &rjlr_filter, "R-Y"};
BUTTON right_joystick_select = {PINOUTS::JOYSTICK_R::SELECT, "R-SEL"};

JOYSTICK left_joystick = {left_joystick_x, left_joystick_y, left_joystick_select};
JOYSTICK right_joystick = {right_joystick_x, right_joystick_y, right_joystick_select};

POT left_trigger = {PINOUTS::TRIGGERS::LT, &lt_filter, "LT"};
BUTTON left_button = {PINOUTS::TRIGGERS::LB, "LB"};
POT right_trigger = {PINOUTS::TRIGGERS::RT, &rt_filter, "RT"};
BUTTON right_button = {PINOUTS::TRIGGERS::RB, "RB"};

//structs to store information about DAC and Gyroscope
DAC dac {&mcp_dac};
GYRO gyro_vals {&sox_gyro};

//to do things iteratively
BUTTON* xyab_buttons[4] = {&x_button, &y_button, &a_button, &b_button};
BUTTON* dpad_buttons[4] = {&dpad_up_button, &dpad_down_button, &dpad_left_button, &dpad_right_button};
BUTTON* misc_buttons[2] = {&misc_home_button, &misc_menu_button};
BUTTON* joystick_buttons[2] = {&left_joystick_select, &right_joystick_select};
BUTTON* trigger_buttons[2] = {&left_button, &right_button};

JOYSTICK* joysticks[2] = {&left_joystick, &right_joystick};
POT* triggers[2] = {&left_trigger, &right_trigger};

// POT joystick_pots[4] = {left_joystick_x, left_joystick_y, right_joystick_x, right_joystick_y};
// //for debugging
// POT oldstick_lr {PINOUTS::OLDSTICK::LR};
// POT oldstick_ud {PINOUTS::OLDSTICK::UD};
//For Adafruit shield, these are default.

unsigned long lastFrame = 0;
int frameRate = 30;

/**
  Enums for easy reading, determines the output mode
*/
enum class OUTPUT_MODE {
  KEYBOARD = 0, XINPUT = 1
};

void init_buttons(BUTTON* button[], size_t size) {
  for(size_t i = 0; i < size; i++) {
    pinMode(button[i]->pin, INPUT_PULLUP);
  }
}


void read_buttons(BUTTON* buttons[], size_t size) {
  for(size_t i = 0; i < size; i++) {
    read_button(buttons[i]);
  }
}

void read_button(BUTTON* button) {
  button->prev_state = button->state;
  button->state = !digitalRead(button->pin);


  button->prev_button_state = button->button_state;
  //setting enum value
  if(!button->prev_state && button->state) {
    button->button_state = BUTTON_STATE::PRESS;
  }
  else if (!button->prev_state && !button->state) {
    button->button_state = BUTTON_STATE::OFF;
  }
  else if(button->prev_state && !button->state) {
    button->button_state = BUTTON_STATE::RELEASE;
  }
  else if(button->prev_state && button->state) {
    button->button_state = BUTTON_STATE::HOLD;
  }
}


void read_and_filter_pot(POT* potentiometer) {
  potentiometer->prev_value = potentiometer->value;
  potentiometer->value = potentiometer->filter->process(analogRead(potentiometer->pin));
  // potentiometer->value = analogRead(potentiometer->pin);
  if(potentiometer->debug) {
    Serial.print(potentiometer->name);
    Serial.print(": ");
    Serial.println(potentiometer->value);
  }
}

void read_gyroscope(GYRO* g) {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  g->gyroscope->getEvent(&accel, &gyro, &temp);
  g->accel_x = accel.acceleration.x;
  g->accel_y = accel.acceleration.y;
  g->accel_z = accel.acceleration.z;
  g->gyro_x = gyro.gyro.x;
  g->gyro_y = gyro.gyro.y;
  g->gyro_z = gyro.gyro.z;
  g->temperature = temp.temperature;
}

void sneb_midi_gyro(GYRO* g) {
  usbMIDI.sendControlChange(1, normalise1(g->gyro_x, -1.0f, 1.0f, 0.0f, 127.0f), 1);
  usbMIDI.sendControlChange(2, normalise1(g->gyro_y, -1.0f, 1.0f, 0.0f, 127.0f), 1);
  usbMIDI.sendControlChange(3, normalise1(g->gyro_z, -1.0f, 1.0f, 0.0f, 127.0f), 1);
  // usbMIDI.sendControlChange(4, g->accel_x, 1);
  // usbMIDI.sendControlChange(5, g->accel_y, 1);
  // usbMIDI.sendControlChange(6, g->accel_z, 1);
  // usbMIDI.sendControlChange(7, g->temperature, 1);
}

void read_joysticks(JOYSTICK* joysticks[], size_t size) {
  for(size_t i = 0; i < size; i++) {
    read_and_filter_pot(&joysticks[i]->x_pot);
    read_and_filter_pot(&joysticks[i]->y_pot);
  }
}

void read_potentiometers(POT* pots[], size_t size) {
  for(size_t i = 0; i < size; i++) {
    read_and_filter_pot(pots[i]);
  }
} 

void press_button(BUTTON* button, int mode) {
  if(button->prev_button_state == BUTTON_STATE::OFF && button->button_state == BUTTON_STATE::PRESS) {

    //keyboard input
    if(static_cast<OUTPUT_MODE>(mode) == OUTPUT_MODE::KEYBOARD) {
        Keyboard.press(button->key_press);
        Keyboard.release(button->key_press);
        delay(10);
    }
  }
  else {
    Keyboard.release(button->key_press);
  }
  // if(button->button_state == BUTTON_STATE::HOLD) {
  //   if(static_cast<OUTPUT_MODE>(mode) == OUTPUT_MODE::KEYBOARD) {
  //     while(button->button_state == BUTTON_STATE::HOLD) {
  //       Keyboard.press(button->key_press);
  //       Keyboard.release(button->key_press);
  //     }
  //   }
  // }
}

void press_misc_midi_test(BUTTON* button, int mode) {
  if(button->button_state == BUTTON_STATE::PRESS) {
    if(static_cast<OUTPUT_MODE>(mode) == OUTPUT_MODE::KEYBOARD) {
      int randNote = rand_int(20, 80);
      usbMIDI.sendNoteOn(randNote, 100, 1);
      delay(150);
      usbMIDI.sendNoteOff(randNote, 100, 1);
    }
  }
}

void DEBUG_potentiometers(POT* pot, int x, int y, int text_color, int bg_color, bool flag = true) {
  String out = pot->name + ": " + pot->value + " ";
  if(pot->prev_value != pot->value) {
    DEBUG_clear_text(out, x, y);
    DEBUG_display_text(out, text_color, bg_color, x, y);
  }
  if(flag) Serial.println(out);
  else Serial.print(out);
}

void setup() {

  // display.cp437(true);
  Serial.begin(9600);
  // XInput.begin();
  Keyboard.begin();
  usbMIDI.begin();

  display.begin();

  // if(!display.begin())
  //   Serial.println("hello silas i did not begin -ili9341 display");

  // display.setFramebuffer(fb_internal);
  // display.setDiffBuffers(&diff1, &diff2);
  // display.setRefreshRate(120); // refresh rate in MHz, frame rate can't exceed (rate) / 2
  // display.setVSyncSpacing(2); // enable vsync and set framerate = refreshrate/2 (typical choice)
  // display.update(fb)

  sox_gyro.begin_I2C(0x6A, &Wire);
  sox_gyro.setGyroRange(LSM6DS_GYRO_RANGE_500_DPS);

  if(!mcp_dac.begin(0x60, &Wire)) {
    Serial.println("Failed to find MCP4728 chip");
    while(true) {
      delay(10);
    }
  }

  Serial.println("We found the MCP4728 DAC!");
  // Serial.println("We found the Gyroscope!");

  mcp_dac.setChannelValue(MCP4728_CHANNEL_A, 4095);
  mcp_dac.setChannelValue(MCP4728_CHANNEL_B, 4095);
  mcp_dac.setChannelValue(MCP4728_CHANNEL_C, 4095);
  mcp_dac.setChannelValue(MCP4728_CHANNEL_D, 4095);

  init_buttons(xyab_buttons, std::size(xyab_buttons));
  init_buttons(dpad_buttons, std::size(dpad_buttons));
  init_buttons(misc_buttons, std::size(misc_buttons));
  init_buttons(joystick_buttons, std::size(joystick_buttons));
  init_buttons(trigger_buttons, std::size(trigger_buttons));

  // DEBUG_buttons(a_button, 100, 100, ILI9341_WHITE, ILI9341_BLACK);

  display.setRotation(4);
  display.fillScreen(ILI9341_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2); //2(6 x 8)
  display.setRotation(4);
  display.println("debug display");
}
int foo = 0;
void loop() {

  read_gyroscope(&gyro_vals);
  sneb_midi_gyro(&gyro_vals);
  

  mcp_dac.fastWrite(normalise1(0, left_trigger.value, 0, 985, 0, 4095), normalise1(0, right_trigger.value, 0, 985, 0, 4095), 4095, 0);
  if(millis() > lastFrame + frameRate) {
    lastFrame = millis();
    for(int i = 0; i < 4; i++) {
      press_button(xyab_buttons[i], static_cast<int>(OUTPUT_MODE::KEYBOARD));
      press_button(dpad_buttons[i], static_cast<int>(OUTPUT_MODE::KEYBOARD));
    }
    read_all_params();
    display_all_params();
    // DEBUG_potentiometers(&joysticks[0]->x_pot, 0, 130 + (16 * 0), ILI9341_WHITE, ILI9341_BLACK, false);
    // DEBUG_potentiometers(&joysticks[0]->y_pot, 120, 130 + (16 * 0), ILI9341_WHITE, ILI9341_BLACK, true);
    // Serial.println();
    // DEBUG_potentiometers(&joysticks[1]->x_pot, 0, 130 + (16 * 1), ILI9341_WHITE, ILI9341_BLACK, false);
    // DEBUG_potentiometers(&joysticks[1]->y_pot, 120, 130 + (16 * 1), ILI9341_WHITE, ILI9341_BLACK, true);
    // DEBUG_potentiometers(triggers[0], 0 + (96 * 0), 226, ILI9341_WHITE, ILI9341_BLACK);
    // DEBUG_potentiometers(triggers[1], 0 + (96 * 1), 226, ILI9341_WHITE, ILI9341_BLACK);
    press_misc_midi_test(&misc_home_button, static_cast<int>(OUTPUT_MODE::KEYBOARD));
    press_misc_midi_test(&misc_menu_button, static_cast<int>(OUTPUT_MODE::KEYBOARD));
  
  }
}

void display_all_params() {
  //arrays of size 4
  for(size_t i = 0; i < 4; i++) {
    DEBUG_buttons(xyab_buttons[i], 0, 50 + (16 * i), ILI9341_WHITE, ILI9341_BLACK);
    DEBUG_buttons(dpad_buttons[i], 84, 50 + (16 * i), ILI9341_WHITE, ILI9341_BLACK);
  }
  //arrays of size 2
  for(size_t i = 0; i < 2; i++) {
    // press_button(misc_buttons[i]);
    DEBUG_potentiometers(&joysticks[i]->x_pot, 0, 130 + (16 * i), ILI9341_WHITE, ILI9341_BLACK, true);
    DEBUG_potentiometers(&joysticks[i]->y_pot, 120, 130 + (16 * i), ILI9341_WHITE, ILI9341_BLACK, true);
    DEBUG_buttons(joystick_buttons[i], 0, 178 + (16 * i), ILI9341_WHITE, ILI9341_BLACK);
    DEBUG_buttons(misc_buttons[i], 134, 178 + (16 * i), ILI9341_WHITE, ILI9341_BLACK);
    DEBUG_buttons(trigger_buttons[i], 0 + (96 * i), 242, ILI9341_WHITE, ILI9341_BLACK);
    DEBUG_potentiometers(triggers[i], 0 + (96 * i), 226, ILI9341_WHITE, ILI9341_BLACK, true);
  }
}

void read_all_params() {
  read_joysticks(joysticks, std::size(joysticks));
  read_potentiometers(triggers, std::size(triggers));

  read_buttons(xyab_buttons, std::size(xyab_buttons));
  read_buttons(dpad_buttons, std::size(dpad_buttons));
  read_buttons(misc_buttons, std::size(misc_buttons));
  read_buttons(joystick_buttons, std::size(joystick_buttons));
  read_buttons(trigger_buttons, std::size(trigger_buttons));
}

/**
 * Helper function that combines the map() and constrain() function into one, guaranteeing that a value from
 * the PIN potentiometer's VALUE is normalised between the values "LOW" to the values "HIGH."
 * @param pin the potentiometer's pin value
 * @param value the variable storing the potentiometer's value
 * @param from the lower bound of value's current range
 * @param to the upper bound of value's upper range
 * @param low the lower bound of the value's target range
 * @param high the upper bound of the value's target range
 * @return a constrained float or int value between the values LOW and HIGH
 */

float normalise (int pin, float value, float from, float to, float low, float high) {
  value = map(analogRead(pin), from, to, low, high);
  value = constrain(value, low, high);
  return value;
}
int normalise(int pin, int value, int from, int to, int low, int high) {
  value = map(analogRead(pin), from, to, low, high);
  value = constrain(value, low, high);
  return value;
}
int normalise1(int value, int to_map, int from, int to, int low, int high) {
  value = map(to_map, from, to, low, high);
  value = constrain(value, low, high);
  return value;
}
float normalise1(float to_map, float from, float to, float low, float high) {
  return constrain(map(to_map, from, to, low, high), low, high);
}

// float rand_float(float from, float to) {
//   return rand() % (to - from + 1) + from;
// }
int rand_int(int from, int to) {
  return rand() % (to - from + 1) + from;
}
/*
  TO-Complete:

  TODO:
  * structs into objects so i can iteratively debug everything more efficiently

*/

