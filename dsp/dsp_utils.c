#include "dsp_utils.h"
#include <math.h>

// midi to freq
float midi_to_freq(int midiNote) {
  return 440.0f * powf(2.0f, ((float)midiNote - 69.0f) / 12.0f);
}

// prevent clipping
float soft_clip(float x) { return x / (1.0f + fabsf(x)); }
