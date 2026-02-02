#include "dsp_api.h"

// Defaults
float g_volume = 1.0f;
bool g_mute = false;

void AudioManager(AppState *myState) {
    env = (ADSR){.attack = myState->adsr.attack,
                .decay = myState->adsr.decay,
                .sustain = myState->adsr.sustain,
                .release = myState->adsr.release,
                .value = 0.0f,
                .stage = ENV_IDLE};

    g_volume = myState->volume;
    g_mute = !myState->audioActive;
}
