#pragma once

class moving_average_filter
{
  float alpha;
  float smoothed_float;
  int smoothed_int;
  bool initialized_float;
  bool initialized_int;

  float bias_float;
  int bias_int;
  public:
    moving_average_filter(float a = 0.2f, float bias = 0) : alpha(a), smoothed_float(0.0f), smoothed_int(0), 
      initialized_float(false), initialized_int(false), bias_float(bias), bias_int(0) {}
    moving_average_filter(float a = 0.2f, int bias = 0) : alpha(a), smoothed_float(0.0f), smoothed_int(0), 
      initialized_float(false), initialized_int(false), bias_float(0), bias_int(bias) {}
    float process(float input);
    int process(int input);
    void set_bias(int val) {bias_int = val;}
    void set_bias(float val) {bias_float = val;}
    float get_bias_float() {return bias_float;}
    int get_bias_int() {return bias_int;}
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
    bool process(float input, float bias = 0.0f);
    bool process(int input, int bias = 0);
};