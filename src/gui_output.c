#include "gui_output.h"
#include "gui_interface.h"
#include "raylib.h"

#include <math.h>
#include <stdio.h>

static const char *GetWaveformName(Waveform f) {
  switch (f) {
  case WAVE_SINE:
    return "Sinusoïdale";
  case WAVE_SQUARE:
    return "Carrée";
  case WAVE_TRIANGLE:
    return "Triangulaire";
  case WAVE_SAWTOOTH:
    return "Dent de scie";
  default:
    return "Inconnue";
  }
}

static float EstimateDb(float volume01) {
  if (volume01 < 1e-6f)
    return -60.0f;
  return 20.0f * log10f(volume01);
}

static void DrawCard(int x, int y, int w, int h, float dpi, const char *label,
                     const char *value, Color valueColor) {
  Rectangle r = {(float)x, (float)y, (float)w, (float)h};
  DrawRectangleRec(r, (Color){235, 235, 235, 255});
  DrawRectangleLinesEx(r, 2, (Color){160, 160, 160, 255});

  DrawText(label, x + (int)(12 * dpi), y + (int)(10 * dpi), (int)(14 * dpi),
           GRAY);
  DrawText(value, x + (int)(12 * dpi), y + (int)(34 * dpi), (int)(18 * dpi),
           valueColor);
}

static void DrawBar(int x, int y, int w, int h, float dpi, float t01,
                    Color fill) {
  if (t01 < 0.0f)
    t01 = 0.0f;
  if (t01 > 1.0f)
    t01 = 1.0f;

  Rectangle r = {(float)x, (float)y, (float)w, (float)h};
  DrawRectangleRec(r, (Color){30, 30, 30, 255});
  DrawRectangleLinesEx(r, 2, (Color){120, 120, 120, 255});

  Rectangle rf = {r.x + 2, r.y + 2, (r.width - 4) * t01, r.height - 4};
  DrawRectangleRec(rf, fill);
}

void DrawOutputPage(AppState *state, int zoneX) {
  float dpi = GetAppDPI();

  Vector2 origin =
      BeginPageContent(zoneX, &state->heightOutput, &state->scrollOutput);

  int mX = (int)(20 * dpi);
  int x = (int)(origin.x + mX);
  int startY = (int)(origin.y);
  int y = startY + (int)(30 * dpi);
  int w = (int)((GetScreenWidth() - zoneX) - 20 * dpi -
                2 * mX); // Width based on contentRec

  DrawText("OUTPUT (MONO)", x, y, (int)(20 * dpi), DARKGRAY);
  y += (int)(30 * dpi);

  int gap = (int)(15 * dpi);
  int boxH = (int)(65 * dpi);
  int colW = (w - gap) / 2;

  char vWaveform[64], vFreq[64], vVol[64], vStatus[64];

  snprintf(vWaveform, sizeof(vWaveform), "%s",
           GetWaveformName(state->waveform));
  snprintf(vFreq, sizeof(vFreq), "%.0f Hz", state->frequencyHz);
  snprintf(vVol, sizeof(vVol), "%.0f %%", state->volume * 100.0f);

  const char *playbackTxt = state->playback ? "PLAY" : "STOP";
  const char *audioTxt = state->audioActive ? "ON" : "OFF";
  snprintf(vStatus, sizeof(vStatus), "%s | AUDIO %s", playbackTxt, audioTxt);

  DrawCard(x, y, colW, boxH, dpi, "Forme", vWaveform, BLACK);
  DrawCard(x + colW + gap, y, colW, boxH, dpi, "Fréquence", vFreq, BLACK);

  y += boxH + gap;

  DrawCard(x, y, colW, boxH, dpi, "Volume", vVol, BLACK);

  Color cStatus = state->audioActive ? (Color){60, 170, 90, 255}
                                     : (Color){210, 80, 80, 255};
  DrawCard(x + colW + gap, y, colW, boxH, dpi, "État", vStatus, cStatus);

  y += boxH + (int)(25 * dpi);

  DrawText("Niveau (estimé)", x, y, (int)(16 * dpi), DARKGRAY);
  y += (int)(22 * dpi);

  float db = EstimateDb(state->volume);
  char vDb[64];
  snprintf(vDb, sizeof(vDb), "%.1f dB", db);
  DrawText(vDb, x, y, (int)(18 * dpi), BLACK);

  y += (int)(28 * dpi);

  DrawBar(x, y, w, (int)(18 * dpi), dpi,
          state->audioActive ? state->volume : 0.0f,
          state->audioActive ? (Color){80, 180, 120, 255}
                             : (Color){200, 70, 70, 255});

  y += (int)(30 * dpi);

  DrawText("Format cible : Mono - 44100 Hz - Float32", x, y, (int)(14 * dpi),
           GRAY);
  y += (int)(18 * dpi);

  EndPageContent((float)y + 50 * dpi, origin.y, &state->heightOutput);
}