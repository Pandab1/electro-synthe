#include "input.h"
#include "raylib.h"
#include "dsp_voice.h"
#include "dsp_utils.h"

#include <math.h>

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

static Waveform NextWave(Waveform w) { return (Waveform)((w + 1) % 4); }

static Waveform PrevWave(Waveform w) { return (Waveform)((w + 3) % 4); }

static float NoteToFreq(int midiNote) {
  return 440.0f * powf(2.0f, (midiNote - 69) / 12.0f);
}

static bool PianoKeyPressed(int key) { return IsKeyPressed(key); }
static bool PianoKeyReleased(int key) { return IsKeyReleased(key); }

void HandleKeyboardShortcuts(AppState *state) {
  // ESC : quit
  if (IsKeyPressed(KEY_ESCAPE)) {
    CloseWindow();
  }
  // Play/Stop
  if (IsKeyPressed(KEY_SPACE)) {
    state->playback = !state->playback;
    state->audioActive = state->playback;
  }

  // Change waveform
  if (IsKeyPressed(KEY_RIGHT))
    state->waveform = NextWave(state->waveform);
  if (IsKeyPressed(KEY_LEFT))
    state->waveform = PrevWave(state->waveform);

  // Volume
  if (IsKeyDown(KEY_UP))
    state->volume += 0.01f;
  if (IsKeyDown(KEY_DOWN))
    state->volume -= 0.01f;

  if (state->volume < 0.0f)
    state->volume = 0.0f;
  if (state->volume > 1.0f)
    state->volume = 1.0f;

  // Frequency
  float step =
      IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ? 10.0f : 1.0f;

  if (IsKeyDown(KEY_KP_ADD) || IsKeyDown(KEY_EQUAL)) {
    state->frequencyHz += step;
  }

  if (IsKeyDown(KEY_KP_SUBTRACT) || IsKeyDown(KEY_MINUS) ||
      (IsKeyDown(KEY_SIX) &&
       (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)))) {
    state->frequencyHz -= step;
  }

  if (state->currentPage == PAGE_PIANO) {

    for (int i = 0; i < 8; i++) {
      if (IsKeyPressed(notes[i].key)) {
        note_on(midi_to_freq(notes[i].midiNote));
        state->playback = true;
        state->audioActive = true;
      }
      if (IsKeyReleased(notes[i].key)) {
        note_off(midi_to_freq(notes[i].midiNote));
        state->playback = false;
      }
    }

    return;
  }
}
