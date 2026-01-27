#define _USE_MATH_DEFINES
#include "custom_math.h"
#include <math.h>

float generate_sin(float time, float freq) {
  return 0.25f * sinf(time * freq * 2.0f * M_PI);
}

float generate_square(float time, float freq) {
  return (sin(2.0 * M_PI * freq * time) >= 0) ? 1.0f : -1.0f;
}

float generate_triangle(float time, float freq) {
  float phase = time * freq;
  float saw = phase - floorf(phase + 0.5f);
  return 2.0f * fabsf(2.0f * saw) - 1.0f;
}

float generate_sawtooth(float time, float freq) {
  float phase = time * freq;
  return 2.0f * (phase - floorf(phase + 0.5f));
}
