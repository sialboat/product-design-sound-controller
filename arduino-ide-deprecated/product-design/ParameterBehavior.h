//turning off so arduino doesn't complain, but this will most likely be depreciated.

#pragma once

/*
  Base class for the behavior implementation of a parameter's macro-level event handling behavior,
  adequately named the "behavior" class. Raw data from a parameter gets passed into this 

*/
class behavior
{

};

// #include <map>

// // Button states to streamline button logic (Steve I wrote my own sort of half-baked implementation of your
// // awesome button please do not yell at me)
// enum class BUTTON_STATE {
//  OFF,       // Button is not pressed. (!state && !prev_state)
//  PRESS,     // Button is pressed (state && !prev_state)
//  HOLD,      // Button is held (state && prev_state)
//  RELEASE    // Button is released (!state && prev_state)
// };

// // Button types to configure button behavior for interpreting button behavior
// enum class BUTTON_TYPE {
//   TAP_HOLD,     // Default button logic (trigger / gate for tap and hold, also tracks release)
//   TOGGLE,       // On/Off button handler (will remain on / off until it is toggled)
//   PROBS,        // Manually determine the probability the button will return an "A" or "B" 
//                 // (chainable for future implementations) upon button press
//   SWITCH        // Cycles (increments, decrements, randomly generates values, etc.) based on Switch behavior.
// };

// //Switch behaviors for how the button will cycle between different values. Hopefully these will be chainable
// //so the user can add as many behaviors as they would like
// enum class SWITCH_TYPE {
//   BIAS,     // Offset cycle behavior by a constant factor
//   SCALE,    // Offset cycle behavior by a linear function
//   WARP,     // Offset cycle behavior by a mathematical function
//   PROBS,    // Adjust the 
//   CYCLE     // Determines the pattern by which the values will cycle (analagous to arpeggio patterns)
// };

// // Behaviors for how Potentiometers will be sent using MIDI.
// enum class POT_BEHAVIOR {
//   X_OP_Y, // Data will be displayed as X <math operation goes here> Y
//   X_OR_Y, // Data will be sent in two separate channels, one for X, one for Y.
//   POLAR,  // Data will be displayed as a Polar Coordinate / in Radians
//   //other stuff here will be stuff for signal processing, Pot controls some sort of values
//   //ie input for Hilbert Transform, paramters for Lorenz attractors, etc.
// };

// // class hosting flags and associated information that allows for easily processing
// // information relating to how parameters are set to behave.
// class param_behavior {
  
// };

// class pot_behavior : public param_behavior {

// };

// class button_behavior : public param_behavior {

// };
