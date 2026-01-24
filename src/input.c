#include "raylib.h"
#include "../include/input.h"

static Waveform NextWave(Waveform w) {
    return (Waveform)((w + 1) % 4);
}

static Waveform PrevWave(Waveform w) {
    return (Waveform)((w + 3) % 4);
}

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
}
