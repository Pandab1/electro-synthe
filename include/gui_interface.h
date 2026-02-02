#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "dsp_oscillator.h"
#include "dsp_adsr.h"
#include "raylib.h"
#include <stdbool.h>

float TextToFloat(const char *text);

typedef enum PageApp {
  PAGE_OSCILLATOR = 0,
  PAGE_ENVELOPE = 1,
  PAGE_OUTPUT = 2,
  PAGE_HELP = 3,
  PAGE_PIANO = 4
} PageApp;

// Global application state (UI + parameters)
typedef struct AppState {
  bool darkMode;
  PageApp currentPage;
  bool audioActive;
  bool playback;
  float visualPhase;
  float volume;
  
  Oscillator osc;
  PlaybackMode playbackMode;

  ADSR adsr;
  Vector2 scrollEnvelope;
  float heightEnvelope;
  Vector2 scrollOscillator;
  float heightOscillator;
  // HELP
  Vector2 scrollHELP;
  float heightHELP;
  Vector2 scrollOutput;
  float heightOutput;

  bool showMessage;
  float sliderValue;
} AppState;

// // Global AppState
// extern AppState myState;

// GUI Functions
void InitGuiStyle(void);
void DrawAppInterface(AppState *state);
float GetAppDPI(void);

Vector2 BeginPageContent(int zoneX, float *contentHeight, Vector2 *scrollState);
void EndPageContent(float finalY, float originY, float *contentHeight);

#endif
