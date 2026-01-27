#include "adsr.h"
#include "config_dsp.h"

ADSR env = {.attack = 0.05f,
            .decay = 0.02f,
            .sustain = 0.8f,
            .release = 0.5f,
            .value = 0.0f,
            .stage = ENV_IDLE};

void adsr_note_on(ADSR *e) { e->stage = ENV_ATTACK; }

void adsr_note_off(ADSR *e) {
  if (e->stage != ENV_IDLE)
    e->stage = ENV_RELEASE;
}

float adsr_process(ADSR *e) {
  switch (e->stage) {
  case ENV_ATTACK:
    e->value += 1.0f / (e->attack * SAMPLE_RATE);
    if (e->value >= 1.0f) {
      e->value = 1.0f;
      e->stage = ENV_DECAY;
    }
    break;

  case ENV_DECAY:
    e->value -= (1.0f - e->sustain) / (e->decay * SAMPLE_RATE);
    if (e->value <= e->sustain) {
      e->value = e->sustain;
      e->stage = ENV_SUSTAIN;
    }
    break;

  case ENV_SUSTAIN:
    break;

  case ENV_RELEASE:
    e->value -= e->sustain / (e->release * SAMPLE_RATE);
    if (e->value <= 0.0f) {
      e->value = 0.0f;
      e->stage = ENV_IDLE;
    }
    break;

  case ENV_IDLE:
    e->value = 0.0f;
    break;
  }

  return e->value;
}
