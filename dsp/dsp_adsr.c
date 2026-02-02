#include "dsp_adsr.h"
#include "dsp_config.h"
#include <math.h>

// Default ADSR env
ADSR env = {.attack = 0.05f,
            .decay = 0.02f,
            .sustain = 0.8f,
            .release = 0.5f,
            .value = 0.0f,
            .stage = ENV_IDLE};

// Helper to calculate rate coefficient
// We use a simplified approximation for the rate.
static float calculate_coef(float timeInSeconds) {
     if (timeInSeconds <= 0.0f) return 1.0f; // Instant
     // Formula: 1 - exp(-1 / (time * SampleRate))
     // Approximate for speed: 1.0 / (time * SR * correction)
     return 1.0f / (timeInSeconds * SAMPLE_RATE);
}

void adsr_note_on(ADSR *e) {
    e->stage = ENV_ATTACK;
}

void adsr_note_off(ADSR *e) {
    if (e->stage != ENV_IDLE) {
        e->stage = ENV_RELEASE;
    }
}

float adsr_process(ADSR *e) {
    float target = 0.0f;
    float rate = 0.0f;

    switch (e->stage) {
    case ENV_IDLE:
        e->value = 0.0f;
        return 0.0f;

    case ENV_ATTACK:
        target = 1.01f; // Target slightly above 1.0 to ensure we actually hit 1.0 quickly (Logarithmic approach never quite reaches target)
        rate = calculate_coef(e->attack) * 3.0f; // Multiplier makes curve convex/punchy
        
        e->value += (target - e->value) * rate;

        if (e->value >= 1.0f) {
            e->value = 1.0f;
            e->stage = ENV_DECAY;
        }
        break;

    case ENV_DECAY:
        target = e->sustain;
        rate = calculate_coef(e->decay);
        
        e->value += (target - e->value) * rate;

        // If we are close enough to sustain level, switch state (optional, can just stay in decay effectively)
        if (fabsf(e->value - e->sustain) < ENV_THRESHOLD) {
             e->value = e->sustain;
             e->stage = ENV_SUSTAIN;
        }
        break;

    case ENV_SUSTAIN:
        // Ensure value tracks sustain if user changes knob while holding note
        e->value += (e->sustain - e->value) * calculate_coef(0.05f); // Smooth transition
        break;

    case ENV_RELEASE:
        target = 0.0f;
        rate = calculate_coef(e->release);
        
        e->value += (target - e->value) * rate;

        if (e->value < ENV_THRESHOLD) {
            e->value = 0.0f;
            e->stage = ENV_IDLE;
        }
        break;
    }

    return e->value;
}
