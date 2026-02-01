#ifndef DSP_VOICE_H
#define DSP_VOICE_H

#include "dsp_adsr.h"
#include "dsp_oscillator.h"

#define MAX_VOICES 8

typedef struct {
  Oscillator osc;
  ADSR env;
  int active;
} Voice;

extern Voice voices[MAX_VOICES];

void voice_init(void);
void note_on(float freq);
void note_off(float freq);

#endif
