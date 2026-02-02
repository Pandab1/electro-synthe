#ifndef DSP_VOICE_H
#define DSP_VOICE_H

#include "dsp_adsr.h"
#include "dsp_oscillator.h"

#define MAX_VOICES 20 // max notes played at once (with the full adsr cycle)

typedef struct {
  Oscillator osc;
  ADSR env;
  int active;
  int midiNote;
} Voice;

extern Voice voices[MAX_VOICES];

void voice_init(void);
void note_on(int midiNote);
void note_off(int midiNote);

#endif
