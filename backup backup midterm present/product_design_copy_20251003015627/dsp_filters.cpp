#include "dsp_filters.h"

float moving_average_filter::process(float input) {
  if(!initialized_float) {
    initialized_float = true;
    smoothed_float = input;
  } else {
    smoothed_float = alpha * input + (1 - alpha) * smoothed_float;
  }
  return smoothed_float + bias_float;
}
int moving_average_filter::process(int input) {
  if(!initialized_int) {
    initialized_int = true;
    smoothed_int = input;
  } else {
    smoothed_int = alpha * input + (1 - alpha) * smoothed_int;
  }
  return smoothed_int + bias_int;
}