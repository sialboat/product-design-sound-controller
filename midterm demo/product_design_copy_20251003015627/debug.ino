#ifndef DEBUG_INO
#define DEBUG_INO

void DEBUG_display_text(String value, int text_color, int bg_color, int x, int y) {
  display.setCursor(x, y);
  display.setTextColor(text_color, bg_color);
  display.print(value);
}
void DEBUG_clear_text(String value, int x, int y) {
  
  display.setCursor(x, y);
  display.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
  for (int i = 0; i < (int) value.length() + 1; i++) {
    display.write(0xDA);
  }
}

void DEBUG_display_value(int value, int text_color, int bg_color, int x, int y) {
  display.setCursor(x, y);
  display.setTextColor(text_color, bg_color);
  display.print(value);
}

void DEBUG_clear_value(int value, int x, int y) {
  display.setCursor(x, y);
  display.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
  for(int i = 0; i < (int) floor(log10(value) + 1 ) + 1; i++) { //apparently this is how you count digits in constant time
    display.write(0xDA);
  }
}

// void DEBUG_potentiometers(POT* pot, int x, int y, int text_color, int bg_color, bool flag = true) {

//   if(pot->prev_value != pot->value) {
//     String out = pot->name + ": " + pot->value + " ";
//     DEBUG_clear_text(out, x, y);
//     DEBUG_display_text(out, text_color, bg_color, x, y);
//     if(flag) Serial.println(out);
//     else Serial.print(out);
//   }
  
// }

// void DEBUG_potentiometers(POT* pot, int x, int y, int text_color, int bg_color, bool flag = true) {
//   String out = pot->name + ": " + pot->value + " ";
//   if(pot->prev_value != pot->value) {
//     DEBUG_clear_text(out, x, y);
//     DEBUG_display_text(out, text_color, bg_color, x, y);
//   }
//   if(flag) Serial.println(out);
//   else Serial.print(out);
// }

void DEBUG_buttons(BUTTON* button, int x, int y, int text_color, int bg_color) {

  if(button->prev_button_state != button->button_state) {
    String out = button->name + ": ";
    out += toString(button->button_state);
    String out_prev = button->name + ": ";
    out_prev += toString(button->prev_button_state);
    DEBUG_clear_text(out, x, y);
    DEBUG_display_text(out, text_color, bg_color, x, y);
    // Serial.println(out);
  } 

}

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