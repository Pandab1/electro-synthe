#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include <stdbool.h>
#include "raylib.h"

float TextToFloat(const char *text);

typedef enum PageApp {
    PAGE_OSCILLATOR = 0,
    PAGE_ENVELOPPE  = 1,
    PAGE_OUTPUT     = 2,
    PAGE_HELP       = 3
} PageApp;

// Formes d'onde (oscillateur)
typedef enum FormeOnde {
    ONDE_SINUS = 0,
    ONDE_CARREE,
    ONDE_TRIANGULAIRE,
    ONDE_DENT_DE_SCIE
} FormeOnde;

typedef enum ModeLecture {
    MODE_CONTINU = 0,
    MODE_ENVELOPPE
} ModeLecture;

typedef struct EnveloppeADSR {
    float attack;   
    float decay;    
    float sustain;  
    float release; 
} EnveloppeADSR;

// Etat global de l'application (UI + paramètres)
typedef struct AppState {
    bool darkMode;
    PageApp pageCourante;
    bool audioActif;
    float phaseVisu;  

    FormeOnde formeOnde;

    // Oscillateur
    float frequenceHz;
    float volume;
    bool lecture;
    ModeLecture modeLecture;

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

// Fonctions GUI
void InitGuiStyle(void);
void DrawAppInterface(AppState *etat);
float GetAppDPI(void);

Vector2 BeginPageContent(int zoneX, float *contentHeight, Vector2 *scrollState);
void EndPageContent(float finalY, float originY, float *contentHeight);

#endif
