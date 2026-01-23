#include "raylib.h"
#include "../include/input.h"

static FormeOnde NextWave(FormeOnde w) {
    return (FormeOnde)((w + 1) % 4);
}

static FormeOnde PrevWave(FormeOnde w) {
    return (FormeOnde)((w + 3) % 4);
}

void HandleKeyboardShortcuts(AppState *etat)
{
    // ESC / Q : quitter 
    if (IsKeyPressed(KEY_Q)) {
        CloseWindow(); 
    }

    // Play/Stop
    if (IsKeyPressed(KEY_SPACE)) {
        etat->lecture = !etat->lecture;
        etat->audioActif = etat->lecture; 
    }

    // changer d’onde
    if (IsKeyPressed(KEY_RIGHT)) etat->formeOnde = NextWave(etat->formeOnde);
    if (IsKeyPressed(KEY_LEFT))  etat->formeOnde = PrevWave(etat->formeOnde);

    // volume 
    if (IsKeyDown(KEY_UP))   etat->volume += 0.01f;
    if (IsKeyDown(KEY_DOWN)) etat->volume -= 0.01f;

    if (etat->volume < 0.0f) etat->volume = 0.0f;
    if (etat->volume > 1.0f) etat->volume = 1.0f;

    // fréquence
    float step = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)  ? 10.0f : 1.0f;
    
    if (IsKeyDown(KEY_KP_ADD) || IsKeyDown(KEY_EQUAL)) {
        etat->frequenceHz += step;
    }

    if (IsKeyDown(KEY_KP_SUBTRACT) || IsKeyDown(KEY_MINUS) || (IsKeyDown(KEY_SIX) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))))
    {
        etat->frequenceHz -= step;
    }
}
