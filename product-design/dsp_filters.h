#pragma once

class moving_average_filter
{
  float alpha;
  float smoothed_float;
  int smoothed_int;
  bool initialized_float;
  bool initialized_int;
  public:
    moving_average_filter(float a) : alpha(a), smoothed_float(0.0f), smoothed_int(0), initialized_float(false), initialized_int(false) {}
    float process(float input);
    int process(int input);
};

class hysteresis_filter
{
  float threshold_low_f;
  float threshold_high_f;
  int threshold_low_i;
  int threshold_high_i;
  
  public:
    hysteresis_filter(float low, float high) : threshold_low_f(low), threshold_high_f(high), threshold_low_i(0), threshold_high_i(0) {}
    hysteresis_filter(int low, int high) : threshold_low_f(0.0f), threshold_high_f(0.0f), threshold_low_i(low), threshold_high_i(high) {}
    bool process(float input);
    bool process(int input);
};