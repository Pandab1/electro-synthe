#include "voice.h"
#include "config_dsp.h"
#include "oscillator.h"
#include <math.h>
#include <stdio.h>

Voice voices[MAX_VOICES];

void voice_init(void) {
  for (int i = 0; i < MAX_VOICES; i++) {
    voices[i].osc.phase = 0.0f;
    voices[i].osc.freq = 0.0f;
    voices[i].osc.sampleRate = SAMPLE_RATE;
    voices[i].osc.waveform = WAVE_SINE;
    voices[i].env = env; // copy default ADSR
    voices[i].active = 0;
  }
}

void note_on(float freq) {
  for (int i = 0; i < MAX_VOICES; i++) {
    if (!voices[i].active) {
      voices[i].osc.freq = freq;
      voices[i].osc.phase = 0.0f;
      voices[i].env = env; // reset ADSR
      adsr_note_on(&voices[i].env);
      voices[i].active = 1;
      printf("Voice %d ON: freq=%.1f\n", i, freq); // Debug output
      break;
    }
  }
}

void note_off(float freq) {
  for (int i = 0; i < MAX_VOICES; i++) {
    if (voices[i].active && fabsf(voices[i].osc.freq - freq) < 0.1f) {
      adsr_note_off(&voices[i].env);
    }
  }
}
