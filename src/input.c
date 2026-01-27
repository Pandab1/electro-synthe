#include "raylib.h"
#include "../include/input.h"

static Waveform NextWave(Waveform w) {
    return (Waveform)((w + 1) % 4);
}

static Waveform PrevWave(Waveform w) {
    return (Waveform)((w + 3) % 4);
}

static float NoteToFreq(int midiNote) {
    return 440.0f * powf(2.0f, (midiNote - 69) / 12.0f);
}

static bool PianoKeyPressed(int key) { return IsKeyPressed(key); }
static bool PianoKeyReleased(int key){ return IsKeyReleased(key); }

void HandleKeyboardShortcuts(AppState *state)
{
    // ESC / Q : quit
    if (IsKeyPressed(KEY_Q)) {
        CloseWindow(); 
    }
    // Play/Stop
    if (IsKeyPressed(KEY_SPACE)) {
        state->playback = !state->playback;
        state->audioActive = state->playback;
    }

    // Change waveform
    if (IsKeyPressed(KEY_RIGHT)) state->waveform = NextWave(state->waveform);
    if (IsKeyPressed(KEY_LEFT))  state->waveform = PrevWave(state->waveform);

    // Volume
    if (IsKeyDown(KEY_UP))   state->volume += 0.01f;
    if (IsKeyDown(KEY_DOWN)) state->volume -= 0.01f;

    if (state->volume < 0.0f) state->volume = 0.0f;
    if (state->volume > 1.0f) state->volume = 1.0f;

    // Frequency
    float step = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)  ? 10.0f : 1.0f;

    if (IsKeyDown(KEY_KP_ADD) || IsKeyDown(KEY_EQUAL)) {
        state->frequencyHz += step;
    }

    if (IsKeyDown(KEY_KP_SUBTRACT) || IsKeyDown(KEY_MINUS) || (IsKeyDown(KEY_SIX) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))))
    {
        state->frequencyHz -= step;
    }

    if (state->currentPage == PAGE_PIANO) {

    // Blanches: Q W E R T Y U -> C D E F G A B (C4..B4)
    if (PianoKeyPressed(KEY_A)) { state->frequencyHz = NoteToFreq(60); state->playback = true; state->audioActive = true; }
    if (PianoKeyPressed(KEY_Z)) { state->frequencyHz = NoteToFreq(62); state->playback = true; state->audioActive = true; }
    if (PianoKeyPressed(KEY_E)) { state->frequencyHz = NoteToFreq(64); state->playback = true; state->audioActive = true; }
    if (PianoKeyPressed(KEY_R)) { state->frequencyHz = NoteToFreq(65); state->playback = true; state->audioActive = true; }
    if (PianoKeyPressed(KEY_T)) { state->frequencyHz = NoteToFreq(67); state->playback = true; state->audioActive = true; }
    if (PianoKeyPressed(KEY_Y)) { state->frequencyHz = NoteToFreq(69); state->playback = true; state->audioActive = true; }
    if (PianoKeyPressed(KEY_U)) { state->frequencyHz = NoteToFreq(71); state->playback = true; state->audioActive = true; }
    {
        state->playback = false;
    }

    return;
    }
}
