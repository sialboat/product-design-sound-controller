#pragma once

template <typename N>
class moving_average_filter{

  private:
  float alpha;
  N smoothed;
  N bias;
  bool initialized;
  
  public:
  moving_average_filter() 
    : alpha(0.1f), smoothed(N{}), bias(N{}), initialized(false) {}
  moving_average_filter(float a, N b) 
    : alpha(a), smoothed(N{}), bias(b), initialized(false) {}
  ~moving_average_filter() = default;


  N process(N input) 
  {
    if(!initialized)
    {
      initialized = true;
      smoothed = input;
    } 
    else 
    {
      smoothed = alpha * input + (1.0f - alpha) * smoothed;
    }
    return smoothed + bias;
  }
  float get_coeff() {return alpha; }
  void set_coeff(float a) {alpha = a;}
  void set_bias(N val) {bias = val;}
  bool is_initialized() { return initialized; }

};

// class moving_average_filter
// {
//   private:
//   float alpha;
//   float smoothed_float;
//   int smoothed_int;
//   bool initialized_float;
//   bool initialized_int;

//   float bias_float;
//   int bias_int;
//   public:
//     moving_average_filter() {
//       alpha = 0.2f;
//       smoothed_float = 0.0f;
//       smoothed_int = 0;
//       initialized_float = false;
//       initialized_int = false;
//       bias_float = 0.0f;
//       bias_int = 0;
//     }
//     moving_average_filter(float a = 0.2f, float bias = 0.0f) : alpha(a), smoothed_float(0.0f), smoothed_int(0), 
//       initialized_float(false), initialized_int(false), bias_float(bias), bias_int(0) {}
//     moving_average_filter(float a = 0.2f, int bias = 0.0f) : alpha(a), smoothed_float(0.0f), smoothed_int(0), 
//       initialized_float(false), initialized_int(false), bias_float(0), bias_int(bias) {}
//     float process(float input);
//     void set_coeff(float c) {alpha = c;}
//     float get_coeff() {return alpha; }
//     int process(int input);
//     void set_bias(int val) {bias_int = val;}
//     void set_bias(float val) {bias_float = val;}
//     float get_bias_float() {return bias_float;}
//     int get_bias_int() {return bias_int;}
// };
// template <typename N>
// N moving_average_filter<N>::process(N input)
// {
  // if(!initialized)
  // {
  //   initialized = true;
  //   smoothed = input;
  // } 
  // else 
  // {
  //   smoothed = alpha * input + (1.0f - alpha) * smoothed;
  // }
  // return smoothed + bias;
// }
// class hysteresis_filter
// {
//   float threshold_low_f;
//   float threshold_high_f;
//   int threshold_low_i;
//   int threshold_high_i;
  
//   public:
//     hysteresis_filter(float low, float high) : threshold_low_f(low), threshold_high_f(high), threshold_low_i(0), threshold_high_i(0) {}
//     hysteresis_filter(int low, int high) : threshold_low_f(0.0f), threshold_high_f(0.0f), threshold_low_i(low), threshold_high_i(high) {}
//     bool process(float input, float bias = 0.0f);
//     bool process(int input, int bias = 0);
// };