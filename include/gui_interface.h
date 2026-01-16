#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include <stdbool.h>

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

// Etat global de l'application (UI + paramètres)
typedef struct AppState {
    bool darkMode;
    PageApp pageCourante;
    bool audioActif;

    FormeOnde formeOnde;

    // Oscillateur
    float frequenceHz;
    float volume;
    bool lecture;

    bool showMessage;
    float sliderValue;
} AppState;

// Fonctions publiques GUI
void InitGuiStyle(void);
void DrawAppInterface(AppState *etat);

#endif