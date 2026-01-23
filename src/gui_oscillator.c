#include "raylib.h"
#include <stdio.h>
#include <math.h>

#include "../include/raygui.h"
#include "../include/gui_interface.h"
#include "../include/gui_components.h"
#include "../include/gui_oscillator.h"

// ---------------------------------------------------------------------
// Calcul de la valeur Y d'une forme d'onde (avec phase animée)
// cyclesVisibles = nombre de périodes affichées sur la largeur du graphe
// phase = décalage (0..1) qui avance dans le temps quand on joue
// ---------------------------------------------------------------------
static float CalculerY(int x, int largeur, float cyclesVisibles, float amp, FormeOnde type, float phase)
{
    // t en "cycles": (x/largeur)*cyclesVisibles + phase
    float t = ((float)x / (float)largeur) * cyclesVisibles + phase;

    float s = 0.0f;

    switch (type) {
        case ONDE_SINUS:
            s = sinf(2.0f * PI * t);
            break;

        case ONDE_CARREE:
            s = (sinf(2.0f * PI * t) >= 0.0f) ? 1.0f : -1.0f;
            break;

        case ONDE_TRIANGULAIRE:
            s = (2.0f / PI) * asinf(sinf(2.0f * PI * t));
            break;

        case ONDE_DENT_DE_SCIE: {
            float frac = t - floorf(t);     // partie fractionnaire [0..1]
            s = 2.0f * frac - 1.0f;         // -1 -> 1
        } break;

        default:
            s = 0.0f;
            break;
    }

    return s * amp;
}

// =====================================================================
// Page Oscillateur
// =====================================================================
void DessinerPageOscillateur(AppState *etat, int zoneX)
{
    float dpi = GetAppDPI();

    Vector2 origin = BeginPageContent(zoneX, &etat->heightOscillator, &etat->scrollOscillator);

    // Assure une taille texte correcte pour les widgets (boutons/slider)
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(16 * dpi));

    int margeX = (int)(20 * dpi);
    // Position relative au scroll
    int x = (int)(origin.x + margeX);
    int w = (int)((GetScreenWidth() - zoneX) - 20 * dpi - 2 * margeX);

    float yOffset = 35.0f * dpi;
    float startY = origin.y + yOffset;

    // -----------------------------
    // Bloc FORMES
    // -----------------------------
    Rectangle zoneForme = { (float)x, startY, (float)w, (60*dpi) };
    DrawRectangleRec(zoneForme, (Color){220,220,220,255});
    DrawRectangleLinesEx(zoneForme, 2, (Color){170,170,170,255});

    DrawText("FORME D'ONDE", (int)(zoneForme.x + 10*dpi), (int)(zoneForme.y + 6*dpi), (int)(14*dpi), DARKGRAY);

    float marge = 10.0f * dpi;
    float espace = 12.0f * dpi;
    float hBtn = 30.0f * dpi;
    float lBtn = (zoneForme.width - 2*marge - 3*espace) / 4.0f;
    float yBtn = zoneForme.y + 24*dpi;

    Rectangle b1 = { zoneForme.x + marge + (lBtn + espace)*0, yBtn, lBtn, hBtn };
    Rectangle b2 = { zoneForme.x + marge + (lBtn + espace)*1, yBtn, lBtn, hBtn };
    Rectangle b3 = { zoneForme.x + marge + (lBtn + espace)*2, yBtn, lBtn, hBtn };
    Rectangle b4 = { zoneForme.x + marge + (lBtn + espace)*3, yBtn, lBtn, hBtn };

    if (DessinerBoutonOnde(b1, ICON_WAVE_SINUS,       "Sinus",    etat->formeOnde == ONDE_SINUS))         etat->formeOnde = ONDE_SINUS;
    if (DessinerBoutonOnde(b2, ICON_WAVE_SQUARE,      "Carrée",   etat->formeOnde == ONDE_CARREE))        etat->formeOnde = ONDE_CARREE;
    if (DessinerBoutonOnde(b3, ICON_WAVE_TRIANGULAR,  "Triangle", etat->formeOnde == ONDE_TRIANGULAIRE))  etat->formeOnde = ONDE_TRIANGULAIRE;
    if (DessinerBoutonOnde(b4, ICON_WAVE,             "Scie",     etat->formeOnde == ONDE_DENT_DE_SCIE))  etat->formeOnde = ONDE_DENT_DE_SCIE;

    //float y = zoneForme.y + zoneForme.height + 10*dpi;
    float currentY = zoneForme.y + zoneForme.height + 10*dpi;


    Rectangle groupeFreq = { (float)x, currentY, (float)w, (50*dpi) };
    DrawRectangleRec(groupeFreq, (Color){220,220,220,255});
    DrawRectangleLinesEx(groupeFreq, 2, (Color){170,170,170,255});

    DessinerSliderAvecBoutons(groupeFreq, "FREQUENCE", &etat->frequenceHz,
                              20.0f, 2000.0f, 10.0f, "%.0f Hz");

    currentY += groupeFreq.height + 10*dpi;

    // Bloc VOLUME
    Rectangle groupeVol = { (float)x, currentY, (float)w, (50*dpi) };
    DrawRectangleRec(groupeVol, (Color){220,220,220,255});
    DrawRectangleLinesEx(groupeVol, 2, (Color){170,170,170,255});

    float volumePourcent = etat->volume * 100.0f;
    DessinerSliderAvecBoutons(groupeVol, "VOLUME", &volumePourcent,
                              0.0f, 100.0f, 5.0f, "%.0f %%");
    etat->volume = volumePourcent / 100.0f;

    currentY += groupeVol.height + 10*dpi;


    Color fond = etat->lecture ? (Color){210, 80, 80, 255} : (Color){90, 180, 110, 255};
    Rectangle btnPlay = { (float)x, currentY, (float)w, (35*dpi) };
    DrawRectangleRec(btnPlay, fond);
    DrawRectangleLinesEx(btnPlay, 2, (Color){60,60,60,255});

    bool isPressed = etat->lecture;

    const char *txt = etat->lecture ? GuiIconText(ICON_PLAYER_STOP, "STOP") : GuiIconText(ICON_PLAYER_PLAY, "PLAY");

    int prevBaseColor = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
    int prevTextColor = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);

    if (etat->lecture) {
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xD25050FF); 
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    } else {
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x5AB46EFF); 
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    }
    if (GuiButton(btnPlay, txt)) {
        etat->lecture = !etat->lecture;

        if (etat->lecture) etat->audioActif = true;
        else              etat->audioActif = false;
    }
    // Restaurer style
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, prevBaseColor);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, prevTextColor);

    currentY += btnPlay.height + 15*dpi;

    DrawText("VISUALISATION", x, (int)(currentY + 5*dpi), (int)(14*dpi), DARKGRAY);
    Rectangle zoneVisu = { (float)x, currentY + 25*dpi, (float)w, (140*dpi) };
    DrawRectangleRec(zoneVisu, (Color){30,30,30,255});
    DrawRectangleLinesEx(zoneVisu, 2, (Color){170,170,170,255});

    // Grille
    for (int i = 1; i < 6; i++) {
        float gx = zoneVisu.x + (zoneVisu.width * i / 6.0f);
        DrawLine((int)gx, (int)zoneVisu.y, (int)gx, (int)(zoneVisu.y + zoneVisu.height), (Color){60,60,60,255});
    }
    DrawLine((int)zoneVisu.x,
             (int)(zoneVisu.y + zoneVisu.height/2),
             (int)(zoneVisu.x + zoneVisu.width),
             (int)(zoneVisu.y + zoneVisu.height/2),
             (Color){70,70,70,255});

    if (etat->lecture && etat->audioActif) {
    float dt = GetFrameTime();
    const float scrollCyclesPerSec = 1.0f; 
    etat->phaseVisu += dt * scrollCyclesPerSec;
    etat->phaseVisu -= floorf(etat->phaseVisu);
    }

    int largeurCourbe = (int)zoneVisu.width - 20;

    // amplitude visuelle
    float amp = (zoneVisu.height * 0.35f) * etat->volume;

    const float windowTime = 0.010f;         
    float cyclesVisibles = etat->frequenceHz * windowTime;

    if (cyclesVisibles < 0.25f) cyclesVisibles = 0.25f;

    for (int i = 0; i < largeurCourbe - 1; i++) {
        float y1 = (zoneVisu.y + zoneVisu.height/2)
                 + CalculerY(i, largeurCourbe, cyclesVisibles, amp, etat->formeOnde, etat->phaseVisu);

        float y2 = (zoneVisu.y + zoneVisu.height/2)
                 + CalculerY(i+1, largeurCourbe, cyclesVisibles, amp, etat->formeOnde, etat->phaseVisu);

        Vector2 p1 = { zoneVisu.x + 10 + (float)i,     y1 };
        Vector2 p2 = { zoneVisu.x + 10 + (float)i + 1, y2 };

        DrawLineV(p1, p2, (Color){180,180,180,255});
    }

    float finalY = currentY + 25*dpi + 140*dpi + 20*dpi; // + 20 dpi padding

    EndPageContent(finalY, origin.y, &etat->heightOscillator);
}
