#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include <stdbool.h>
#include "raylib.h"

float TextToFloat(const char *text);

typedef enum PageApp {
    PAGE_OSCILLATOR = 0,
    PAGE_ENVELOPE  = 1,
    PAGE_OUTPUT     = 2,
    PAGE_HELP       = 3
} PageApp;

// Waveforms (oscillator)
typedef enum Waveform {
    WAVE_SINE = 0,
    WAVE_SQUARE,
    WAVE_TRIANGLE,
    WAVE_SAWTOOTH
} Waveform;

typedef enum PlaybackMode {
    MODE_CONTINUOUS = 0,
    MODE_ENVELOPE
} PlaybackMode;

typedef struct EnvelopeADSR {
    float attack;
    float decay;    
    float sustain;  
    float release; 
} EnvelopeADSR;

// Global application state (UI + parameters)
typedef struct AppState {
    bool darkMode;
    PageApp currentPage;
    bool audioActive;
    float visualPhase;

    Waveform waveform;

    // Oscillator
    float frequencyHz;
    float volume;
    bool playback;
    PlaybackMode playbackMode;

    EnvelopeADSR adsr;
    Vector2 scrollEnvelope;
    float heightEnvelope;
    Vector2 scrollOscillator;
    float heightOscillator;
    //HELP
    Vector2 scrollHELP;
    float heightHELP;
    Vector2 scrollOutput;
    float heightOutput;

    EnveloppeADSR adsr;
    Vector2 scrollEnveloppe;
    float heightEnveloppe;
    Vector2 scrollOscillator;
    float heightOscillator;
    Vector2 scrollOutput;
    float heightOutput;

    bool showMessage;
    float sliderValue;
} AppState;

// GUI Functions
void InitGuiStyle(void);
void DrawAppInterface(AppState *state);
float GetAppDPI(void);

Vector2 BeginPageContent(int zoneX, float *contentHeight, Vector2 *scrollState);
void EndPageContent(float finalY, float originY, float *contentHeight);

#endif
