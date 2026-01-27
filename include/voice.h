#ifndef VOICE_H
#define VOICE_H

#include "adsr.h"
#include "oscillator.h"

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
