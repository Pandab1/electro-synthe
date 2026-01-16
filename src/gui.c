#include "raylib.h"
#include <stdlib.h>

// Polyfills for Raylib 5.0 compatibility
float TextToFloat(const char *text) {
    return strtof(text, NULL);
}

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

#include "../include/gui_interface.h"

void InitGuiStyle(void) {
    // GuiLoadStyle("styles/test.rgs");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
}

void DrawAppInterface(AppState *state) {
    // test du state
    if (state->darkMode) {
        ClearBackground(DARKGRAY);
    } else {
        ClearBackground(RAYWHITE);
    }

    DrawText("Electro-Synthe GUI", 20, 20, 20, state->darkMode ? WHITE : BLACK);

    // Bouton
    if (GuiButton((Rectangle){ 25, 60, 140, 30 }, "Activer Message")) {
        state->showMessage = !state->showMessage;
    }

    // Checkbox
    GuiCheckBox((Rectangle){ 25, 110, 20, 20 }, "Mode Sombre", &state->darkMode);

    // Slider
    GuiSlider((Rectangle){ 25, 150, 120, 20 }, "0", "100", &state->sliderValue, 0.0f, 100.0f);

    // Logique d'affichage
    if (state->showMessage) {
        DrawText(TextFormat("Valeur: %.2f", state->sliderValue), 200, 60, 20, BLUE);

        // Exemple d'un élément visuel
        DrawRectangleLines(200, 100, 100, 100, RED);
        DrawText("Signal", 220, 140, 10, RED);
    }
}