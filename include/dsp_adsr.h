#ifndef DSP_ADSR_H
#define DSP_ADSR_H

typedef enum {
  ENV_IDLE,
  ENV_ATTACK,
  ENV_DECAY,
  ENV_SUSTAIN,
  ENV_RELEASE
} EnvStage;

typedef struct {
  float attack;
  float decay;
  float sustain;
  float release;
  float value;
  EnvStage stage;
} ADSR;

void adsr_note_on(ADSR *e);
void adsr_note_off(ADSR *e);
float adsr_process(ADSR *e);

extern ADSR env;

#endif
