#include "dsp_api.h"

void AudioManager(AppState *myState, ADSR *env) {
    *env = (ADSR){.attack = myState->adsr.attack,
                .decay = myState->adsr.decay,
                .sustain = myState->adsr.sustain,
                .release = myState->adsr.release,
                .value = 0.0f,
                .stage = ENV_IDLE};
}
