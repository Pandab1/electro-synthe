#ifndef DSP_API_H
#define DSP_API_H

#include "gui_interface.h"

extern float g_volume;
extern bool g_mute;
extern bool g_continuous;
extern Oscillator g_osc;

void AudioManager(AppState *state);

#endif