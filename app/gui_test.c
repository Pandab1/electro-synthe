#include "config_dsp.h"
#include "miniaudio.h"
#include "oscillator.h"
#include "raylib.h"
#include "utils_dsp.h"
#include "voice.h"
#include <stdlib.h>

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

void keyboard(void) {
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

void get_visual_buffer(float *dst, int count) {
  int writeIndex = visWriteIndex;
  for (int i = 0; i < count; i++)
    dst[i] =
        visBuffer[(writeIndex - count + i + VIS_BUFFER_SIZE) % VIS_BUFFER_SIZE];
}

void data_callback(ma_device *device, void *output, const void *input,
                   ma_uint32 frameCount) {
  float *out = (float *)output;
  for (ma_uint32 i = 0; i < frameCount; i++)
    out[i] = synth_next_sample();
}

int main(void) {
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
