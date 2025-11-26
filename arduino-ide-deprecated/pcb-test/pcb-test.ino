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
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240

typedef struct Button {
  const int pin;
  String name;
  int key_press;

  bool state = false;
  bool prev_state = true;   //so we can initially debug and display the thing

  // BUTTON_STATE button_state = BUTTON_STATE::OFF;
  // BUTTON_STATE prev_button_state = BUTTON_STATE::HOLD;
  bool debug = false;
  bool pressed = false;
} BUTTON;

// void display_all_params() {
//   //arrays of size 4
//   for(size_t i = 0; i < 4; i++) {
//     DEBUG_buttons(xyab_buttons[i], 0, 50 + (16 * i), ILI9341_WHITE, ILI9341_BLACK);
//     DEBUG_buttons(dpad_buttons[i], 84, 50 + (16 * i), ILI9341_WHITE, ILI9341_BLACK);
//   }
//   //arrays of size 2
//   for(size_t i = 0; i < 2; i++) {
//     // press_button(misc_buttons[i]);
//     DEBUG_potentiometers(&joysticks[i]->x_pot, 0, 130 + (16 * i), ILI9341_WHITE, ILI9341_BLACK, true);
//     DEBUG_potentiometers(&joysticks[i]->y_pot, 120, 130 + (16 * i), ILI9341_WHITE, ILI9341_BLACK, true);
//     DEBUG_buttons(joystick_buttons[i], 0, 178 + (16 * i), ILI9341_WHITE, ILI9341_BLACK);
//     DEBUG_buttons(misc_buttons[i], 134, 178 + (16 * i), ILI9341_WHITE, ILI9341_BLACK);
//     DEBUG_buttons(trigger_buttons[i], 0 + (96 * i), 242, ILI9341_WHITE, ILI9341_BLACK);
//     DEBUG_potentiometers(triggers[i], 0 + (96 * i), 226, ILI9341_WHITE, ILI9341_BLACK, true);
//   }
// }

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

Adafruit_ILI9341 display = Adafruit_ILI9341(PINOUTS::TFT_DISPLAY::CS, PINOUTS::TFT_DISPLAY::DC);

//to do things iteratively
BUTTON* xyab_buttons[4] = {&x_button, &y_button, &a_button, &b_button};
BUTTON* dpad_buttons[4] = {&dpad_up_button, &dpad_down_button, &dpad_left_button, &dpad_right_button};
BUTTON* misc_buttons[2] = {&misc_home_button, &misc_menu_button};
// BUTTON* joystick_buttons[2] = {&left_joystick_select, &right_joystick_select};
// BUTTON* trigger_buttons[2] = {&left_button, &right_button};

void read_button(BUTTON* button) {
  button->prev_state = button->state;
  button->state = !digitalRead(button->pin);
  if(button->state) {
    Serial.println(button->name);
  }


  // button->prev_button_state = button->button_state;
  //setting enum value
  // if(!button->prev_state && button->state) {
  //   button->button_state = BUTTON_STATE::PRESS;
  // }
  // else if (!button->prev_state && !button->state) {
  //   button->button_state = BUTTON_STATE::OFF;
  // }
  // else if(button->prev_state && !button->state) {
  //   button->button_state = BUTTON_STATE::RELEASE;
  // }
  // else if(button->prev_state && button->state) {
  //   button->button_state = BUTTON_STATE::HOLD;
  // }
}


void read_buttons(BUTTON* buttons[], size_t size) {
  for(size_t i = 0; i < size; i++) {
    read_button(buttons[i]);
  }
}

// void read_and_filter_pot(POT* potentiometer) {
//   potentiometer->prev_value = potentiometer->value;
//   potentiometer->value = potentiometer->filter->process(analogRead(potentiometer->pin));
//   // Serial.println(potentiometer->value);
//   // potentiometer->value = analogRead(potentiometer->pin);
//   if(potentiometer->debug) {
//     Serial.print(potentiometer->name);
//     Serial.print(": ");
//     Serial.println(potentiometer->value);
//   }
// }

// void read_gyroscope(GYRO* g) {
//   sensors_event_t accel;
//   sensors_event_t gyro;
//   sensors_event_t temp;
//   g->gyroscope->getEvent(&accel, &gyro, &temp);
//   g->accel_x = accel.acceleration.x;
//   g->accel_y = accel.acceleration.y;
//   g->accel_z = accel.acceleration.z;
//   g->gyro_x = gyro.gyro.x;
//   g->gyro_y = gyro.gyro.y;
//   g->gyro_z = gyro.gyro.z;
//   g->temperature = temp.temperature;
// }

// void sneb_midi_gyro(GYRO* g) {
//   usbMIDI.sendControlChange(1, normalise1(g->gyro_x, -1.0f, 1.0f, 0.0f, 127.0f), 1);
//   usbMIDI.sendControlChange(2, normalise1(g->gyro_y, -1.0f, 1.0f, 0.0f, 127.0f), 1);
//   usbMIDI.sendControlChange(3, normalise1(g->gyro_z, -1.0f, 1.0f, 0.0f, 127.0f), 1);
//   usbMIDI.sendControlChange(4, normalise1(g->accel_x, -1.0f, 1.0f, 0.0f, 127.0f), 1);
//   usbMIDI.sendControlChange(5, normalise1(g->accel_y, -1.0f, 1.0f, 0.0f, 127.0f), 1);
//   usbMIDI.sendControlChange(6, normalise1(g->accel_z, -1.0f, 1.0f, 0.0f, 127.0f), 1);
//   // usbMIDI.sendControlChange(7, g->temperature, 1);
// }

// void sned_midi_cc_pot(POT* p, int chan, int low, int high) {
//   usbMIDI.sendControlChange(chan, normalise1(p->value, low, high, 0s, 127), 1);
// }

// void read_joysticks(JOYSTICK* joysticks[], size_t size) {
//   for(size_t i = 0; i < size; i++) {
//     read_and_filter_pot(&joysticks[i]->x_pot);
//     read_and_filter_pot(&joysticks[i]->y_pot);
//   }
// }

// void read_potentiometers(POT* pots[], size_t size) {
//   for(size_t i = 0; i < size; i++) {
//     read_and_filter_pot(pots[i]);
//   }
// } 

void press_button(BUTTON* button, int mode) {
  // simplified
  // if( (button->prev_button_state == BUTTON_STATE::OFF && button->button_state == BUTTON_STATE::PRESS) || 
      // (button->prev_button_state == BUTTON_STATE::HOLD && button->button_state == BUTTON_STATE::HOLD)) {
    // if(!button->pressed) {
    //   Serial.println("AAAA");
    //   Serial.println(button->name);
    //   // Keyboard.press(button->key_press);
    //   button->pressed = true;
    // } else {
    if(button->pressed) {
      // Keyboard.release(button->key_press);
      Serial.println("BBBB");
      Serial.println(button->name);
      button->pressed = false;
    }
  // }
  // delay(20);
}

void init_buttons(BUTTON* button[], size_t size) {
  for(size_t i = 0; i < size; i++) {
    pinMode(button[i]->pin, INPUT_PULLUP);
  }
}
int bypassPin = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(PINOUTS::JOYSTICK_L::SELECT, INPUT_PULLUP);
  pinMode(PINOUTS::JOYSTICK_R::SELECT, INPUT_PULLUP);
  pinMode(bypassPin, INPUT_PULLUP);
  init_buttons(xyab_buttons, std::size(xyab_buttons));
  init_buttons(dpad_buttons, std::size(dpad_buttons));
  init_buttons(misc_buttons, std::size(misc_buttons));
  // init_buttons(joystick_buttons, std::size(joystick_buttons));
  // init_buttons(trigger_buttons, std::size(trigger_buttons));
  // display.begin();
  // display.setRotation(4);
  // display.fillScreen(ILI9341_BLACK);
  // display.setCursor(0, 0);
  // display.setTextColor(ILI9341_WHITE);
  // display.setTextSize(2); //2(6 x 8)
  // display.setRotation(4);
  // display.println("debug display");

}

void loop()
{
  // Serial.println(digitalRead(PINOUTS::JOYSTICK_L::SELECT));
  // Serial.println(analogRead(PINOUTS::JOYSTICK_L::XOUT));
  // Serial.println(analogRead(PINOUTS::JOYSTICK_L::YOUT));
  Serial.println(digitalRead(PINOUTS::JOYSTICK_R::SELECT));
  Serial.println(analogRead(PINOUTS::JOYSTICK_R::XOUT));
  Serial.println(analogRead(PINOUTS::JOYSTICK_R::YOUT));
  delay(100);
  // Serial.println(digitalRead);
  // Serial.println("foo");
  // Serial.println(digitalRead(bypassPin));
  // read_buttons(xyab_buttons, std::size(xyab_buttons));
  // read_buttons(dpad_buttons, std::size(dpad_buttons));
  // read_buttons(misc_buttons, std::size(misc_buttons));

}




// void DEBUG_display_text(String value, int text_color, int bg_color, int x, int y) {
//   display.setCursor(x, y);
//   display.setTextColor(text_color, bg_color);
//   display.print(value);
// }
// void DEBUG_clear_text(String value, int x, int y) {
  
//   display.setCursor(x, y);
//   display.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
//   for (int i = 0; i < (int) value.length() + 1; i++) {
//     display.write(0xDA);
//   }
// }

// void DEBUG_display_value(int value, int text_color, int bg_color, int x, int y) {
//   display.setCursor(x, y);
//   display.setTextColor(text_color, bg_color);
//   display.print(value);
// }

// void DEBUG_clear_value(int value, int x, int y) {
//   display.setCursor(x, y);
//   display.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
//   for(int i = 0; i < (int) floor(log10(value) + 1 ) + 1; i++) { //apparently this is how you count digits in constant time
//     display.write(0xDA);
//   }
// }

