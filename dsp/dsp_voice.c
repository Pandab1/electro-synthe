#include "dsp_voice.h"
#include "dsp_config.h"
#include "dsp_oscillator.h"
#include "dsp_utils.h"
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

void note_on(int midiNote) {
  float freq = midi_to_freq(midiNote);
  for (int i = 0; i < MAX_VOICES; i++) {
    if (!voices[i].active) {
      voices[i].osc.freq = freq;
      voices[i].osc.phase = 0.0f;
      voices[i].env = env; // reset ADSR
      adsr_note_on(&voices[i].env);
      voices[i].active = 1;
      voices[i].midiNote = midiNote;
      printf("Voice %d ON: freq=%.2f\n", i, freq); // Debug output
      break;
    }
  }
}

void note_off(int midiNote) {
  for (int i = 0; i < MAX_VOICES; i++) {
    if (voices[i].active && voices[i].midiNote == midiNote) {
      adsr_note_off(&voices[i].env);
      printf("Voice %d OFF: freq=%.2f\n", i,
             midi_to_freq(midiNote)); // Debug output
    }
  }
}
