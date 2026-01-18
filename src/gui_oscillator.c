#include "raylib.h"
#include <math.h>

#include "../include/gui_interface.h"
#include "../include/gui_components.h"
#include "../include/gui_oscillator.h"

// Calcul de la valeur Y d'une forme d'onde
static float CalculerY(int x, int largeur, float freq, float amp, FormeOnde type)
{
    float t = (float)x / (float)largeur;
    float s = 0.0f;

    if (type == ONDE_SINUS) {
        s = sinf(2.0f * PI * freq * t);
    } else if (type == ONDE_CARREE) {
        s = (sinf(2.0f * PI * freq * t) >= 0.0f) ? 1.0f : -1.0f;
    } else if (type == ONDE_TRIANGULAIRE) {
        // triangle simple via asin(sin)
        s = (2.0f / PI) * asinf(sinf(2.0f * PI * freq * t));
    } else if (type == ONDE_DENT_DE_SCIE) {
        // sawtooth : rampe -1 -> 1
        float phase = fmodf(freq * t, 1.0f);
        s = 2.0f * phase - 1.0f;
    }

    return s * amp;
}

//======================================================================
// Draw the page
//======================================================================
void DessinerPageOscillateur(AppState *etat, int zoneX)
{
    float dpi = GetAppDPI();

    int margeX = (int)(20 * dpi);
    int x = zoneX + margeX;
    int w = GetScreenWidth() - zoneX - 2*margeX;

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

    if (DessinerBoutonOnde(b1, "~", "Sinus",      etat->formeOnde == ONDE_SINUS))         etat->formeOnde = ONDE_SINUS;
    if (DessinerBoutonOnde(b2, "∏", "Carrée",     etat->formeOnde == ONDE_CARREE))        etat->formeOnde = ONDE_CARREE;
    if (DessinerBoutonOnde(b3, "△", "Triangle",   etat->formeOnde == ONDE_TRIANGULAIRE))  etat->formeOnde = ONDE_TRIANGULAIRE;
    if (DessinerBoutonOnde(b4, "/", "Scie",       etat->formeOnde == ONDE_DENT_DE_SCIE))  etat->formeOnde = ONDE_DENT_DE_SCIE;

    float y = zoneForme.y + zoneForme.height + 10*dpi;


    Rectangle groupeFreq = { (float)x, y, (float)w, (float)(50*dpi) };
    DrawRectangleRec(groupeFreq, (Color){220,220,220,255});
    DrawRectangleLinesEx(groupeFreq, 2, (Color){170,170,170,255});

    DessinerSliderAvecBoutons(groupeFreq, "FREQUENCE", &etat->frequenceHz,
                              20.0f, 2000.0f, 10.0f, "%.0f Hz");

    y += groupeFreq.height + 10*dpi;


    Rectangle groupeVol = { (float)x, y, (float)w, (float)(50*dpi) };
    DrawRectangleRec(groupeVol, (Color){220,220,220,255});
    DrawRectangleLinesEx(groupeVol, 2, (Color){170,170,170,255});

    float volumePourcent = etat->volume * 100.0f;
    DessinerSliderAvecBoutons(groupeVol, "VOLUME", &volumePourcent,
                              0.0f, 100.0f, 5.0f, "%.0f %%");
    etat->volume = volumePourcent / 100.0f;

    y += groupeVol.height + 10*dpi;


    Rectangle btnPlay = { (float)x, y, (float)w, (float)(35*dpi) };

    Color fond = etat->lecture ? (Color){210, 80, 80, 255} : (Color){90, 180, 110, 255};
    DrawRectangleRec(btnPlay, fond);
    DrawRectangleLinesEx(btnPlay, 2, (Color){60,60,60,255});

    const char *txt = etat->lecture ? "■ STOP" : "▶ PLAY";
    int tw = MeasureText(txt, (int)(18*dpi));
    DrawText(txt, (int)(btnPlay.x + btnPlay.width/2 - tw/2), (int)(btnPlay.y + 8*dpi), (int)(18*dpi), RAYWHITE);

    if (CheckCollisionPointRec(GetMousePosition(), btnPlay) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        etat->lecture = !etat->lecture;
    }

    y += btnPlay.height + 15*dpi;

    DrawText("VISUALISATION", x, (int)(y + 5*dpi), (int)(14*dpi), DARKGRAY);

    Rectangle zoneVisu = { (float)x, y + 25*dpi, (float)w, (float)(140*dpi) };
    DrawRectangleRec(zoneVisu, (Color){30,30,30,255});
    DrawRectangleLinesEx(zoneVisu, 2, (Color){170,170,170,255});

    for (int i = 1; i < 6; i++) {
        float gx = zoneVisu.x + (zoneVisu.width * i / 6.0f);
        DrawLine((int)gx, (int)zoneVisu.y, (int)gx, (int)(zoneVisu.y + zoneVisu.height), (Color){60,60,60,255});
    }
    DrawLine((int)zoneVisu.x, (int)(zoneVisu.y + zoneVisu.height/2),
             (int)(zoneVisu.x + zoneVisu.width), (int)(zoneVisu.y + zoneVisu.height/2),
             (Color){70,70,70,255});

    // Courbe
    int largeurCourbe = (int)zoneVisu.width - 20;
    float amp = (zoneVisu.height * 0.35f) * etat->volume;   // volume influence l'amplitude
    float freqAff = etat->frequenceHz / 200.0f;             // réduire pour l'affichage

    for (int i = 0; i < largeurCourbe - 1; i++) {
        float y1 = (zoneVisu.y + zoneVisu.height/2) + CalculerY(i, 120, freqAff, amp, etat->formeOnde);
        float y2 = (zoneVisu.y + zoneVisu.height/2) + CalculerY(i+1, 120, freqAff, amp, etat->formeOnde);

        Vector2 p1 = { zoneVisu.x + 10 + (float)i, y1 };
        Vector2 p2 = { zoneVisu.x + 10 + (float)i + 1, y2 };

        DrawLineV(p1, p2, (Color){180,180,180,255});
    }
}
