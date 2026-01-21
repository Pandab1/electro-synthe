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

    // Assure une taille texte correcte pour les widgets (boutons/slider)
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(16 * dpi));

    int margeX = (int)(20 * dpi);
    int x = zoneX + margeX;
    int w = GetScreenWidth() - zoneX - 2*margeX;

    // -----------------------------
    // Bloc FORMES
    // -----------------------------
    DrawText("FORME D'ONDE", x, (int)(35*dpi), (int)(14*dpi), DARKGRAY);

    Rectangle zoneForme = { (float)x, (float)(50*dpi), (float)w, (float)(45*dpi) };
    DrawRectangleRec(zoneForme, (Color){220,220,220,255});
    DrawRectangleLinesEx(zoneForme, 2, (Color){170,170,170,255});

    float marge = 10.0f * dpi;
    float espace = 12.0f * dpi;
    float hBtn = 30.0f * dpi;
    float lBtn = (zoneForme.width - 2*marge - 3*espace) / 4.0f;

    Rectangle b1 = { zoneForme.x + marge + (lBtn + espace)*0, zoneForme.y + 7.5f*dpi, lBtn, hBtn };
    Rectangle b2 = { zoneForme.x + marge + (lBtn + espace)*1, zoneForme.y + 7.5f*dpi, lBtn, hBtn };
    Rectangle b3 = { zoneForme.x + marge + (lBtn + espace)*2, zoneForme.y + 7.5f*dpi, lBtn, hBtn };
    Rectangle b4 = { zoneForme.x + marge + (lBtn + espace)*3, zoneForme.y + 7.5f*dpi, lBtn, hBtn };

    if (DessinerBoutonOnde(b1, "~", "Sinusoïdale",  etat->formeOnde == ONDE_SINUS))        etat->formeOnde = ONDE_SINUS;
    if (DessinerBoutonOnde(b2, "∏", "Carrée",       etat->formeOnde == ONDE_CARREE))       etat->formeOnde = ONDE_CARREE;
    if (DessinerBoutonOnde(b3, "△", "Triangulaire", etat->formeOnde == ONDE_TRIANGULAIRE)) etat->formeOnde = ONDE_TRIANGULAIRE;
    if (DessinerBoutonOnde(b4, "/", "Dent de scie", etat->formeOnde == ONDE_DENT_DE_SCIE)) etat->formeOnde = ONDE_DENT_DE_SCIE;

    float y = zoneForme.y + zoneForme.height + 10*dpi;

    // -----------------------------
    // Bloc FREQUENCE
    // -----------------------------
    Rectangle groupeFreq = { (float)x, y, (float)w, (float)(50*dpi) };
    DrawRectangleRec(groupeFreq, (Color){220,220,220,255});
    DrawRectangleLinesEx(groupeFreq, 2, (Color){170,170,170,255});

    DessinerSliderAvecBoutons(groupeFreq, "FREQUENCE", &etat->frequenceHz,
                              20.0f, 2000.0f, 10.0f, "%.0f Hz");

    y += groupeFreq.height + 10*dpi;

    // -----------------------------
    // Bloc VOLUME
    // -----------------------------
    Rectangle groupeVol = { (float)x, y, (float)w, (float)(50*dpi) };
    DrawRectangleRec(groupeVol, (Color){220,220,220,255});
    DrawRectangleLinesEx(groupeVol, 2, (Color){170,170,170,255});

    float volumePourcent = etat->volume * 100.0f;
    DessinerSliderAvecBoutons(groupeVol, "VOLUME", &volumePourcent,
                              0.0f, 100.0f, 5.0f, "%.0f %%");
    etat->volume = volumePourcent / 100.0f;

    y += groupeVol.height + 10*dpi;

    // -----------------------------
    // Bouton PLAY/STOP
    // -----------------------------
    Rectangle btnPlay = { (float)x, y, (float)w, (float)(35*dpi) };

    Color fond = etat->lecture ? (Color){210, 80, 80, 255} : (Color){90, 180, 110, 255};
    DrawRectangleRec(btnPlay, fond);
    DrawRectangleLinesEx(btnPlay, 2, (Color){60,60,60,255});

    const char *txt = etat->lecture ? "■ STOP" : "▶ PLAY";
    int tw = MeasureText(txt, (int)(18*dpi));
    DrawText(txt,
             (int)(btnPlay.x + btnPlay.width/2 - tw/2),
             (int)(btnPlay.y + 8*dpi),
             (int)(18*dpi),
             RAYWHITE);

    if (CheckCollisionPointRec(GetMousePosition(), btnPlay) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        etat->lecture = !etat->lecture;

        if (etat->lecture) etat->audioActif = true;
        else              etat->audioActif = false;
    }

    y += btnPlay.height + 15*dpi;

    // -----------------------------
    // VISUALISATION (animée)
    // -----------------------------
    DrawText("VISUALISATION", x, (int)(y + 5*dpi), (int)(14*dpi), DARKGRAY);

    Rectangle zoneVisu = { (float)x, y + 25*dpi, (float)w, (float)(140*dpi) };
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

    // Phase qui avance (défilement) uniquement si on joue ET moteur audio ON
    if (etat->lecture && etat->audioActif) {
    float dt = GetFrameTime();
    const float scrollCyclesPerSec = 1.0f; // vitesse du défilement (constante)
    etat->phaseVisu += dt * scrollCyclesPerSec;
    etat->phaseVisu -= floorf(etat->phaseVisu);
    }

    // Courbe
    int largeurCourbe = (int)zoneVisu.width - 20;

    // amplitude visuelle
    float amp = (zoneVisu.height * 0.35f) * etat->volume;

    // nombre de cycles visibles à l’écran 
    // "timebase" fixe : plus la fréquence monte, plus ça se resserre (zoom horizontal)
    const float windowTime = 0.010f;           // 10 ms affichés sur la largeur
    float cyclesVisibles = etat->frequenceHz * windowTime;

    // clamp bas uniquement (évite que ce soit trop plat à basse fréquence)
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

}
