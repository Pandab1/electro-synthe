#include "custom_math.h"
#include "utils_maths.h"
#include <math.h>

f32 generate_sin(f32 time, f32 freq) {
  return 0.25f * sinf(time * freq * 2.0f * PI);
}

f32 generate_square(f32 time, f32 freq, int ampl) {
  return (sin(2.0 * PI * freq * time) >= 0) ? ampl : -ampl;
}

f32 generate_triangle(f32 time, f32 freq) {
  f32 phase = time * freq;
  f32 saw = phase - floorf(phase + 0.5f);
  return 2.0f * fabsf(2.0f * saw) - 1.0f;
}

f32 generate_sawtooth(f32 time, f32 freq) {
  f32 phase = time * freq;
  return 2.0f * (phase - floorf(phase + 0.5f));
}
