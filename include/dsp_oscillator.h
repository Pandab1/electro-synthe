#ifndef DSP_OSCILLATOR_H
#define DSP_OSCILLATOR_H

#include "dsp_config.h"

typedef enum { WAVE_SINE, WAVE_SQUARE, WAVE_SAW, WAVE_TRIANGLE } Waveform;

typedef struct {
  float phase;
  float freq;
  float sampleRate;
  Waveform waveform;
} Oscillator;

// buffer for later vizualisation with raylib
extern float visBuffer[VIS_BUFFER_SIZE];
extern int visWriteIndex;

float osc_next_sample(Oscillator *o);
float synth_next_sample(void);

#endif
