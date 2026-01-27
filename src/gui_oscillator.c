#include "raylib.h"
#include <math.h>
#include <stdio.h>

#include "gui_components.h"
#include "gui_interface.h"
#include "gui_oscillator.h"
#include "raygui.h"

// ---------------------------------------------------------------------
// Calculate Y value of a waveform (with animated phase)
// visibleCycles = number of periods displayed on the graph width
// phase = offset (0..1) that advances in time when playing
// ---------------------------------------------------------------------
static float CalculateY(int x, int width, float visibleCycles, float amp,
                        Waveform type, float phase) {
  // t in "cycles": (x/width)*visibleCycles + phase
  float t = ((float)x / (float)width) * visibleCycles + phase;

  float s = 0.0f;

  switch (type) {
  case WAVE_SINE:
    s = sinf(2.0f * PI * t);
    break;

  case WAVE_SQUARE:
    s = (sinf(2.0f * PI * t) >= 0.0f) ? 1.0f : -1.0f;
    break;

  case WAVE_TRIANGLE:
    s = (2.0f / PI) * asinf(sinf(2.0f * PI * t));
    break;

  case WAVE_SAWTOOTH: {
    float frac = t - floorf(t); // fractional part [0..1]
    s = 2.0f * frac - 1.0f;     // -1 -> 1
  } break;

  default:
    s = 0.0f;
    break;
  }

  return s * amp;
}

// =====================================================================
// Oscillator Page
// =====================================================================
void DrawOscillatorPage(AppState *state, int zoneX) {
  float dpi = GetAppDPI();

  Vector2 origin = BeginPageContent(zoneX, &state->heightOscillator,
                                    &state->scrollOscillator);

  // Ensure correct text size for widgets (buttons/slider)
  GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(16 * dpi));

  int marginX = (int)(20 * dpi);
  // Position relative to scroll
  int x = (int)(origin.x + marginX);
  int w = (int)((GetScreenWidth() - zoneX) - 20 * dpi - 2 * marginX);

  float yOffset = 35.0f * dpi;
  float startY = origin.y + yOffset;

  // -----------------------------
  // WAVEFORMS Block
  // -----------------------------
  Rectangle waveformZone = {(float)x, startY, (float)w, (60 * dpi)};
  DrawRectangleRec(waveformZone, (Color){220, 220, 220, 255});
  DrawRectangleLinesEx(waveformZone, 2, (Color){170, 170, 170, 255});

  DrawText("FORME D'ONDE", (int)(waveformZone.x + 10 * dpi),
           (int)(waveformZone.y + 6 * dpi), (int)(14 * dpi), DARKGRAY);

  float margin = 10.0f * dpi;
  float spacing = 12.0f * dpi;
  float hBtn = 30.0f * dpi;
  float wBtn = (waveformZone.width - 2 * margin - 3 * spacing) / 4.0f;
  float yBtn = waveformZone.y + 24 * dpi;

  Rectangle b1 = {waveformZone.x + margin + (wBtn + spacing) * 0, yBtn, wBtn,
                  hBtn};
  Rectangle b2 = {waveformZone.x + margin + (wBtn + spacing) * 1, yBtn, wBtn,
                  hBtn};
  Rectangle b3 = {waveformZone.x + margin + (wBtn + spacing) * 2, yBtn, wBtn,
                  hBtn};
  Rectangle b4 = {waveformZone.x + margin + (wBtn + spacing) * 3, yBtn, wBtn,
                  hBtn};

  if (DrawWaveButton(b1, ICON_WAVE_SINUS, "Sinus",
                     state->waveform == WAVE_SINE))
    state->waveform = WAVE_SINE;
  if (DrawWaveButton(b2, ICON_WAVE_SQUARE, "Carrée",
                     state->waveform == WAVE_SQUARE))
    state->waveform = WAVE_SQUARE;
  if (DrawWaveButton(b3, ICON_WAVE_TRIANGULAR, "Triangle",
                     state->waveform == WAVE_TRIANGLE))
    state->waveform = WAVE_TRIANGLE;
  if (DrawWaveButton(b4, ICON_WAVE, "Scie", state->waveform == WAVE_SAWTOOTH))
    state->waveform = WAVE_SAWTOOTH;

  float currentY = waveformZone.y + waveformZone.height + 10 * dpi;

  Rectangle freqGroup = {(float)x, currentY, (float)w, (50 * dpi)};
  DrawRectangleRec(freqGroup, (Color){220, 220, 220, 255});
  DrawRectangleLinesEx(freqGroup, 2, (Color){170, 170, 170, 255});

  DrawSliderWithButtons(freqGroup, "FREQUENCE", &state->frequencyHz, 20.0f,
                        2000.0f, 10.0f, "%.0f Hz");

  currentY += freqGroup.height + 10 * dpi;

  // VOLUME Block
  Rectangle volGroup = {(float)x, currentY, (float)w, (50 * dpi)};
  DrawRectangleRec(volGroup, (Color){220, 220, 220, 255});
  DrawRectangleLinesEx(volGroup, 2, (Color){170, 170, 170, 255});

  float volumePercent = state->volume * 100.0f;
  DrawSliderWithButtons(volGroup, "VOLUME", &volumePercent, 0.0f, 100.0f, 5.0f,
                        "%.0f %%");
  state->volume = volumePercent / 100.0f;

  currentY += volGroup.height + 10 * dpi;

  Color background =
      state->playback ? (Color){210, 80, 80, 255} : (Color){90, 180, 110, 255};
  Rectangle btnPlay = {(float)x, currentY, (float)w, (35 * dpi)};
  DrawRectangleRec(btnPlay, background);
  DrawRectangleLinesEx(btnPlay, 2, (Color){60, 60, 60, 255});

  const char *txt = state->playback ? GuiIconText(ICON_PLAYER_STOP, "STOP")
                                    : GuiIconText(ICON_PLAYER_PLAY, "PLAY");

  int prevBaseColor = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
  int prevTextColor = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);

  if (state->playback) {
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xD25050FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
  } else {
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x5AB46EFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
  }
  if (GuiButton(btnPlay, txt)) {
    state->playback = !state->playback;

    if (state->playback)
      state->audioActive = true;
    else
      state->audioActive = false;
  }
  // Restore style
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, prevBaseColor);
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, prevTextColor);

  currentY += btnPlay.height + 15 * dpi;

  DrawText("VISUALISATION", x, (int)(currentY + 5 * dpi), (int)(14 * dpi),
           DARKGRAY);
  Rectangle visualizationZone = {(float)x, currentY + 25 * dpi, (float)w,
                                 (140 * dpi)};
  DrawRectangleRec(visualizationZone, (Color){30, 30, 30, 255});
  DrawRectangleLinesEx(visualizationZone, 2, (Color){170, 170, 170, 255});

  // Grid
  for (int i = 1; i < 6; i++) {
    float gx = visualizationZone.x + (visualizationZone.width * i / 6.0f);
    DrawLine((int)gx, (int)visualizationZone.y, (int)gx,
             (int)(visualizationZone.y + visualizationZone.height),
             (Color){60, 60, 60, 255});
  }
  DrawLine((int)visualizationZone.x,
           (int)(visualizationZone.y + visualizationZone.height / 2),
           (int)(visualizationZone.x + visualizationZone.width),
           (int)(visualizationZone.y + visualizationZone.height / 2),
           (Color){70, 70, 70, 255});

  if (state->playback && state->audioActive) {
    float dt = GetFrameTime();
    const float scrollCyclesPerSec = 1.0f;
    state->visualPhase += dt * scrollCyclesPerSec;
    state->visualPhase -= floorf(state->visualPhase);
  }

  int curveWidth = (int)visualizationZone.width - 20;

  // visual amplitude
  float amp = (visualizationZone.height * 0.35f) * state->volume;

  const float windowTime = 0.010f;
  float visibleCycles = state->frequencyHz * windowTime;

  if (visibleCycles < 0.25f)
    visibleCycles = 0.25f;

  for (int i = 0; i < curveWidth - 1; i++) {
    float y1 = (visualizationZone.y + visualizationZone.height / 2) +
               CalculateY(i, curveWidth, visibleCycles, amp, state->waveform,
                          state->visualPhase);

    float y2 = (visualizationZone.y + visualizationZone.height / 2) +
               CalculateY(i + 1, curveWidth, visibleCycles, amp,
                          state->waveform, state->visualPhase);

    Vector2 p1 = {visualizationZone.x + 10 + (float)i, y1};
    Vector2 p2 = {visualizationZone.x + 10 + (float)i + 1, y2};

    DrawLineV(p1, p2, (Color){180, 180, 180, 255});
  }

  float finalY = currentY + 25 * dpi + 140 * dpi + 20 * dpi; // + 20 dpi padding

  EndPageContent(finalY, origin.y, &state->heightOscillator);
}
