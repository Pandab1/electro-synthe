#include "dsp_api.h"

// Defaults
float g_volume = 1.0f;
bool g_mute = false;
bool g_continuous = true;
Oscillator g_osc = {
    .freq = 0.0f,
    .phase = 0.0f,
    .sampleRate = SAMPLE_RATE,
    .waveform = WAVE_SINE,

};

void AudioManager(AppState *myState) {
  // update ADSR env
  env.attack = myState->adsr.attack;
  env.decay = myState->adsr.decay;
  env.sustain = myState->adsr.sustain;
  env.release = myState->adsr.release;

  // update global osc
  g_osc.freq = myState->osc.freq;
  g_osc.waveform = myState->osc.waveform;

  // other
  g_volume = myState->volume;
  g_mute = !myState->audioActive;
  g_continuous = g_continuous = (myState->playbackMode == MODE_CONTINUOUS);
}
