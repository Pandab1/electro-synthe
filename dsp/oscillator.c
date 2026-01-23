#include "oscillator.h"
#include "config_dsp.h"
#include "utils_dsp.h"
#include "voice.h"
#include <math.h>

int visWriteIndex = 0;
float visBuffer[VIS_BUFFER_SIZE] = {0};

// Returns the next sample from the oscillator
float osc_next_sample(Oscillator *o) {
  o->phase += o->freq / o->sampleRate;
  if (o->phase >= 1.0f)
    o->phase -= 1.0f;

  switch (o->waveform) {
  case WAVE_SINE:
    return sinf(2.0f * M_PI * o->phase);

  case WAVE_SQUARE:
    return (o->phase < 0.5f) ? 1.0f : -1.0f;

  case WAVE_SAW:
    return 2.0f * o->phase - 1.0f;

  case WAVE_TRIANGLE:
    return 4.0f * fabsf(o->phase - 0.5f) - 1.0f;
  }

  return 0.0f;
}

// Synth
float synth_next_sample(void) {
  float mix = 0.0f;

  for (int i = 0; i < MAX_VOICES; i++) {
    if (!voices[i].active)
      continue;

    float oscSample = osc_next_sample(&voices[i].osc);
    float envValue = adsr_process(&voices[i].env);

    mix += oscSample * envValue;
    mix = soft_clip(mix);

    if (voices[i].env.stage == ENV_IDLE)
      voices[i].active = 0;
  }

  int index = visWriteIndex++;
  visBuffer[index % VIS_BUFFER_SIZE] = mix;

  return mix;
}
