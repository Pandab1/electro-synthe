#include "miniaudio.h"
#include "raylib.h"
#include <math.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VOICES 8
#define SAMPLE_RATE 44100.0f
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

typedef enum { WAVE_SINE, WAVE_SQUARE, WAVE_SAWTOOTH, WAVE_TRIANGLE } Waveform;

typedef struct {
  float phase;
  float freq;
  float sampleRate;
  Waveform waveform;
} Oscillator;

typedef struct {
  Oscillator osc;
  ADSR env;
  int active;
} Voice;

ADSR env = {.attack = 0.05f,
            .decay = 0.02f,
            .sustain = 0.8f,
            .release = 0.5f,
            .value = 0.0f,
            .stage = ENV_IDLE};

Voice voices[MAX_VOICES];

void voice_init() {
  for (int i = 0; i < MAX_VOICES; i++) {
    voices[i].osc.phase = 0.0f;
    voices[i].osc.freq = 0.0f;
    voices[i].osc.sampleRate = SAMPLE_RATE;
    voices[i].osc.waveform = WAVE_SAWTOOTH;
    voices[i].env = env; // copy default ADSR
    voices[i].active = 0;
  }
}

void adsr_note_on(ADSR *e) { e->stage = ENV_ATTACK; }

void adsr_note_off(ADSR *e) {
  if (e->stage != ENV_IDLE)
    e->stage = ENV_RELEASE;
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

// notes
static inline float midi_to_freq(int midiNote) {
  return 440.0f * powf(2.0f, (midiNote - 69) / 12.0f);
}

typedef struct {
  KeyboardKey key;
  int midiNote;
} KeyNote;

KeyNote notes[] = {
    {KEY_Q, 72}, // C5
    {KEY_W, 74}, // D5
    {KEY_E, 76}, // E5
    {KEY_R, 77}, // F5
    {KEY_T, 79}, // G5
    {KEY_Y, 81}, // A5
    {KEY_U, 83}, // B5
    {KEY_I, 84}  // C6
};

void keyboard() {
  static int octaveOffset = 0;
  if (IsKeyPressed(KEY_UP))
    octaveOffset--;
  if (IsKeyPressed(KEY_DOWN))
    octaveOffset++;
  if (octaveOffset < -2)
    octaveOffset = -2;
  if (octaveOffset > 2)
    octaveOffset = 2;

  for (int i = 0; i < 8; i++) {
    if (IsKeyPressed(notes[i].key)) {
      // note_on(midi_to_freq(notes[i].midiNote));
      note_on(midi_to_freq(notes[i].midiNote + octaveOffset * 12));
    }
    if (IsKeyReleased(notes[i].key)) {
      note_off(midi_to_freq(notes[i].midiNote + octaveOffset * 12));
    }
  }
}

// buffer
#define VIS_BUFFER_SIZE 1024
float visBuffer[VIS_BUFFER_SIZE];
_Atomic int visWriteIndex = 0;

// Returns the next sample from the oscillator
float osc_next_sample(Oscillator *o) {
  o->phase += o->freq / o->sampleRate;
  if (o->phase >= 1.0f)
    o->phase -= 1.0f;

  switch (o->waveform) {
  case WAVE_SINE:
    return sinf(2.0f * PI * o->phase);

  case WAVE_SQUARE:
    return (o->phase < 0.5f) ? 1.0f : -1.0f;

  case WAVE_SAWTOOTH:
    return 2.0f * o->phase - 1.0f;

  case WAVE_TRIANGLE:
    return 4.0f * fabsf(o->phase - 0.5f) - 1.0f;
  }

  return 0.0f;
}

// clip
static inline float soft_clip(float x) { return x / (1.0f + fabsf(x)); }

// Synth
float synth_next_sample() {
  float mix = 0.0f;

  for (int i = 0; i < MAX_VOICES; i++) {
    if (!voices[i].active)
      continue;

    float oscSample = osc_next_sample(&voices[i].osc);
    float envValue = adsr_process(&voices[i].env);

    mix += oscSample * envValue;
    mix = soft_clip(mix);

    if (voices[i].env.stage == ENV_IDLE)
      voices[i].active = 0;
  }

  int index = atomic_fetch_add(&visWriteIndex, 1);
  visBuffer[index % VIS_BUFFER_SIZE] = mix;

  return mix;
}

// MINIAUDIO
void data_callback(ma_device *device, void *output, const void *input,
                   ma_uint32 frameCount) {
  float *out = (float *)output;
  for (ma_uint32 i = 0; i < frameCount; i++)
    out[i] = synth_next_sample();
}

void get_visual_buffer(float *dst, int count) {
  int writeIndex = atomic_load(&visWriteIndex);
  for (int i = 0; i < count; i++)
    dst[i] =
        visBuffer[(writeIndex - count + i + VIS_BUFFER_SIZE) % VIS_BUFFER_SIZE];
}

int main() {
  voice_init();

  ma_device_config config = ma_device_config_init(ma_device_type_playback);
  config.playback.format = ma_format_f32;
  config.playback.channels = 1;
  config.sampleRate = SAMPLE_RATE;
  config.dataCallback = data_callback;

  ma_device device;
  if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
    return -1;
  if (ma_device_start(&device) != MA_SUCCESS)
    return -1;

  InitWindow(1600, 800, "Synth Oscilloscope (audio réel)");
  SetTargetFPS(60);

  float samples[VIS_BUFFER_SIZE];

  while (!WindowShouldClose()) {

    keyboard();
    get_visual_buffer(samples, VIS_BUFFER_SIZE);

    BeginDrawing();
    ClearBackground(BLACK);

    int w = GetScreenWidth();
    int h = GetScreenHeight();
    float gain = 300.0f;

    for (int x = 0; x < w - 1; x++) {
      int i1 = (x * VIS_BUFFER_SIZE) / w;
      int i2 = ((x + 1) * VIS_BUFFER_SIZE) / w;

      float y1 = samples[i1];
      float y2 = samples[i2];

      int py1 = h / 2 - (int)(y1 * gain);
      int py2 = h / 2 - (int)(y2 * gain);

      DrawLine(x, py1, x + 1, py2, GREEN);
    }

    EndDrawing();
  }

  CloseWindow();
  ma_device_uninit(&device);
  return 0;
}