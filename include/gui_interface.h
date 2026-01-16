#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "raylib.h"
typedef struct {
    bool showMessage;
    bool darkMode;
    float sliderValue;
} AppState;

// Prototypes des fonctions
void InitGuiStyle(void);
void DrawAppInterface(AppState *state);

#endif