#include "raylib.h"
#include <math.h>

#include "../include/gui_interface.h"
#include "../include/gui_components.h"
#include "../include/gui_enveloppe.h"

// Fonction interne pour dessiner la visualisation de l'enveloppe
static void DessinerVisualisationADSR(AppState *etat, Rectangle zone)
{
    DrawRectangleRec(zone, (Color){20, 20, 20, 255}); // Fond sombre
    DrawRectangleLinesEx(zone, 2, (Color){170, 170, 170, 255});

    float marge = 20.0f;
    float hUtile = zone.height - (marge * 2);
    float wUtile = zone.width - (marge * 2);

    // Calcul des proportions (Somme totale pour l'axe X)
    // On normalise par rapport aux max : Attack(2s) + Decay(2s) + Sustain(fixe) + Release(3s)
    float totalTemps = etat->adsr.attack + etat->adsr.decay + 1.0f + etat->adsr.release;
    
    // Points clés (X, Y)
    Vector2 pStart = { zone.x + marge, zone.y + zone.height - marge };
    
    // Attack
    Vector2 pAttack = {
        pStart.x + (etat->adsr.attack / totalTemps) * wUtile,
        zone.y + marge
    };
    
    // Decay
    Vector2 pDecay = {
        pAttack.x + (etat->adsr.decay / totalTemps) * wUtile,
        pAttack.y + (1.0f - etat->adsr.sustain) * hUtile
    };
    
    // Sustain (on lui donne une largeur fixe pour la visibilité)
    Vector2 pSustainEnd = {
        pDecay.x + (1.0f / totalTemps) * wUtile,
        pDecay.y
    };
    
    // Release
    Vector2 pEnd = {
        pSustainEnd.x + (etat->adsr.release / totalTemps) * wUtile,
        pStart.y
    };

    // Dessin des lignes de l'enveloppe
    DrawLineV(pStart, pAttack, (Color){130, 180, 220, 255});
    DrawLineV(pAttack, pDecay, (Color){130, 180, 220, 255});
    DrawLineV(pDecay, pSustainEnd, (Color){130, 180, 220, 255});
    DrawLineV(pSustainEnd, pEnd, (Color){130, 180, 220, 255});

    // Libellés A, D, S, R sous le graphique
    int fontSize = 12;
    DrawText("A", (int)pStart.x, (int)(zone.y + zone.height - 15), fontSize, GRAY);
    DrawText("D", (int)pAttack.x, (int)(zone.y + zone.height - 15), fontSize, GRAY);
    DrawText("S", (int)pDecay.x, (int)(zone.y + zone.height - 15), fontSize, GRAY);
    DrawText("R", (int)pSustainEnd.x, (int)(zone.y + zone.height - 15), fontSize, GRAY);
}

void DessinerPageEnveloppe(AppState *etat, int zoneX)
{
    float dpi = GetAppDPI();
    int margeX = (int)(20 * dpi);
    int x = zoneX + margeX;
    int w = GetScreenWidth() - zoneX - 2 * margeX;
    float y = 25 * dpi;

    // --- SECTION 1 : EXPLICATION ---
    Rectangle zoneInfo = { (float)x, y, (float)w, (float)(100 * dpi) };
    DrawRectangleRec(zoneInfo, (Color){235, 235, 235, 255});
    DrawRectangleLinesEx(zoneInfo, 1, (Color){180, 180, 180, 255});

    DrawText("ENVELOPPE ADSR", x + 10, (int)(y + 10), (int)(12 * dpi), DARKGRAY);
    DrawText("Attack: Temps pour atteindre le volume max", x + 10, (int)(y + 30), (int)(11 * dpi), GRAY);
    DrawText("Decay: Temps pour descendre au niveau de sustain", x + 10, (int)(y + 50), (int)(11 * dpi), GRAY);
    DrawText("Sustain: Niveau maintenu pendant la note", x + 10, (int)(y + 70), (int)(11 * dpi), GRAY);
    DrawText("Release: Temps pour revenir au silence", x + 10, (int)(y + 90), (int)(11 * dpi), GRAY);

    y += zoneInfo.height + 15 * dpi;

    // --- SECTION 2 : SLIDERS ---
    float hSlider = 70.0f * dpi;
    float espacement = 10.0f * dpi;

    // ATTACK
    Rectangle rAttack = { (float)x, y, (float)w, hSlider };
    DrawRectangleRec(rAttack, (Color){230, 230, 230, 255});
    DessinerSliderAvecBoutons(rAttack, "ATTACK TIME", &etat->adsr.attack, 0.0f, 2.0f, 0.05f, "%.2fs");
    y += hSlider + espacement;

    // DECAY
    Rectangle rDecay = { (float)x, y, (float)w, hSlider };
    DrawRectangleRec(rDecay, (Color){230, 230, 230, 255});
    DessinerSliderAvecBoutons(rDecay, "DECAY TIME", &etat->adsr.decay, 0.0f, 2.0f, 0.05f, "%.2fs");
    y += hSlider + espacement;

    // SUSTAIN
    Rectangle rSustain = { (float)x, y, (float)w, hSlider };
    DrawRectangleRec(rSustain, (Color){230, 230, 230, 255});
    DessinerSliderAvecBoutons(rSustain, "SUSTAIN LEVEL", &etat->adsr.sustain, 0.0f, 1.0f, 0.05f, "%.2f");
    y += hSlider + espacement;

    // RELEASE
    Rectangle rRelease = { (float)x, y, (float)w, hSlider };
    DrawRectangleRec(rRelease, (Color){230, 230, 230, 255});
    DessinerSliderAvecBoutons(rRelease, "RELEASE TIME", &etat->adsr.release, 0.0f, 3.0f, 0.05f, "%.2fs");
    y += hSlider + espacement;

    // --- SECTION 3 : VISUALISATION ---
    DrawText("VISUALISATION ADSR", x, (int)(y + 5), (int)(12 * dpi), DARKGRAY);
    Rectangle zoneVisu = { (float)x, y + 25 * dpi, (float)w, (float)(120 * dpi) };
    DessinerVisualisationADSR(etat, zoneVisu);
}