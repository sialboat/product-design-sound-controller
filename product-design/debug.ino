#ifndef DEBUG_INO
#define DEBUG_INO

#include "font_tgx_OpenSans.h"


// void DEBUG_display_text(String value, int x, int y, color_t color = tgx::RGB565_White) {
//   im.drawText(value.c_str(), {x, y}, color, font_tgx_OpenSans_14, color)
//   display.update(fb);
// }
// void DEBUG_clear_text() {
//   im.clear(tgx::RGB565_Black);
//   display.update(fb);`
// }

// void DEBUG_display_value(int value, int x, int y, color_t color = tgx::RGB565_White) {
//   im.drawText((char*) value, {x, y}, font_tgx_OpenSans_14, color);
//   display.update(fb);
// }

// void DEBUG_clear_value() {
//   im.clear(tgx::RGB565_Black);
//   display.update(fb);
// }

// void DEBUG_potentiometers(POT* pot, int x, int y, color_t text_color = tgx::RGB565_White) {
//   if(pot->prev_value != pot->value) {
//     String out = pot->name + ": " + pot->value;
//     DEBUG_display_text(out.c_str(), x, y, color);
//     display.update(fb);
//     Serial.println(out);
//   }
  
// }

// void DEBUG_buttons(BUTTON* button, int x, int y, color_t text_color = tgx::RGB565_White) {
//   String out = button->name + ": ";
//   out += toString(button->button_state);
//   String out_prev = button->name + ": ";
//   out_prev += toString(button->prev_button_state);

//   if(button->prev_button_state != button->button_state) {
//     // DEBUG_clear_text(out, x, y);
//     DEBUG_display_text(out.c_str(), x, y, text_color);
//     display.update();
//   } 

// }

String toString(BUTTON_STATE b) {
  switch(b) {
    case BUTTON_STATE::PRESS: return "TAP";
    case BUTTON_STATE::HOLD: return "HLD";
    case BUTTON_STATE::RELEASE: return "RLS";
    case BUTTON_STATE::OFF: return "OFF";
    default: return "N/A";
  }
}

// template<size_t N>
// void read_buttons (BUTTON (&arr)[N]) {
//   for(size_t i = 0; i < N; i++) {
//     arr[i].prev_state = arr[i].state;
//     arr[i].state = digitalRead(arr[i].pin);
//   }
// }

// template<size_t N>
// void initialize_buttons(BUTTON (&arr)[N]) {
//   for(size_t i = 0; i < N; i++) {
//     pinMode(arr[i].pin, INPUT);
//   }
// }

// void read_and_filter_pot(POT potentiometer) {
//   potentiometer.value = potentiometer.filter.process(analogRead(potentiometer.pin));
//   if(potentiometer.debug) {
//     Serial.print(potentiometer.name);
//     Serial.print(": ");
//     Serial.println(potentiometer.value);
//   }
// }

// template <size_t N>
// void read_joysticks(POT (&arr)[N]) {
//   for(size_t i = 0; i < N; i++) {
//     read_and_filter_pot(arr[i].x_pot);
//     read_and_filter_pot(arr[i].y_pot);
//   }
// }

// template<size_t N>
// void read_triggers (POT (&arr)[N]) {
//   for(size_t i = 0; i < N; i++) {
//     read_and_filter_pot(arr[i]);
//   }
// }
// void read_joysticks(JOYSTICK joysticks[]) {
//   for(int i = 0; i < (int) (sizeof(joysticks) / sizeof(joysticks)); i++) {
//     read_and_filter_pot(joysticks[i].x_pot);
//     read_and_filter_pot(joysticks[i].y_pot);
//   }
// }
// void read_triggers (POT triggers[]) {
//   for(int i = 0; i < (int) (sizeof(triggers) / sizeof(triggers[0])); i++) {
//     read_and_filter_pot(triggers[i]);
//   }
// }

#endif