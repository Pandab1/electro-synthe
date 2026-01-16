#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#include "../include/gui_interface.h"

// Prototype AVANT raygui
float TextToFloat(const char *text);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"
#pragma GCC diagnostic pop

// Polyfills for Raylib 5.0 compatibility

// Implémentation
float TextToFloat(const char *text)
{
    return strtof(text, NULL);
}


static void DessinerBoutonMenu(AppState *etat, const char *libelle, PageApp page,
                              int x, int y, int largeur, int hauteur);

static void DessinerMenuLateral(AppState *etat);

static bool DessinerBoutonOnde(Rectangle zone, const char *icone,
                               const char *libelle, bool selectionne);

static void DessinerPageOscillateur(AppState *etat, int zoneX);

static void DessinerSliderAvecBoutons(Rectangle zoneGroupe,
                                      const char *titre,
                                      float *valeur,
                                      float min, float max,
                                      float pas,
                                      const char *formatTexte)
{
    // Titre
    DrawText(titre, (int)zoneGroupe.x + 10, (int)zoneGroupe.y + 10, 20, DARKGRAY);

    // Bouton -
    Rectangle btnMoins = { zoneGroupe.x + 10, zoneGroupe.y + 45, 40, 40 };
    if (GuiButton(btnMoins, "-")) {
        *valeur -= pas;
        if (*valeur < min) *valeur = min;
    }

    // Bouton +
    Rectangle btnPlus = { zoneGroupe.x + zoneGroupe.width - 50, zoneGroupe.y + 45, 40, 40 };
    if (GuiButton(btnPlus, "+")) {
        *valeur += pas;
        if (*valeur > max) *valeur = max;
    }

    // Slider central
    Rectangle slider = { zoneGroupe.x + 60, zoneGroupe.y + 55, zoneGroupe.width - 120, 20 };
    GuiSlider(slider, "", "", valeur, min, max);

    // Texte valeur au centre sous le slider
    const char *txt = TextFormat(formatTexte, *valeur);
    int w = MeasureText(txt, 22);
    DrawText(txt, (int)(zoneGroupe.x + zoneGroupe.width/2 - w/2), (int)zoneGroupe.y + 80, 22, DARKGRAY);

    // Bornes min / max
    DrawText(TextFormat(formatTexte, min), (int)(zoneGroupe.x + 60), (int)zoneGroupe.y + 80, 20, GRAY);
    int wMax = MeasureText(TextFormat(formatTexte, max), 20);
    DrawText(TextFormat(formatTexte, max), (int)(zoneGroupe.x + zoneGroupe.width - 60 - wMax), (int)zoneGroupe.y + 80, 20, GRAY);
}

//======================================================================
// Dessine un bouton du menu (style différent si sélectionné)
//======================================================================
static void DessinerBoutonMenu(AppState *etat, const char *libelle, PageApp page,
                              int x, int y, int largeur, int hauteur)
{
    bool selectionne = (etat->pageCourante == page);

    if (selectionne) {
        DrawRectangle(x, y, largeur, hauteur, (Color){130, 180, 220, 255});
        DrawRectangleLines(x, y, largeur, hauteur, (Color){90, 140, 180, 255});
    } else {
        DrawRectangle(x, y, largeur, hauteur, (Color){200, 200, 200, 255});
        DrawRectangleLines(x, y, largeur, hauteur, (Color){160, 160, 160, 255});
    }

    // Zone cliquable
    if (GuiButton((Rectangle){(float)x, (float)y, (float)largeur, (float)hauteur}, libelle)) {
        etat->pageCourante = page;
    }
}

//======================================================================
// Menu latéral : navigation + bloc "moteur audio"
//======================================================================
static void DessinerMenuLateral(AppState *etat)
{
    const int largeurMenu = 260;

    // Réduire temporairement la taille du texte pour la sidebar
    int prevTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    int sidebarTextSize = (int)(prevTextSize * 0.75f);
    GuiSetStyle(DEFAULT, TEXT_SIZE, sidebarTextSize);

    DrawRectangle(0, 0, largeurMenu, GetScreenHeight(), (Color){220, 220, 220, 255});
    DrawText("MENU", 20, 20, 24, (Color){70, 70, 70, 255});

    const int xBouton = 15;
    const int largeurBouton = largeurMenu - 30;
    const int hauteurBouton = 55;
    const int espace = 15;

    int y = 60;

    DessinerBoutonMenu(etat, "Oscillator", PAGE_OSCILLATOR, xBouton, y, largeurBouton, hauteurBouton); y += hauteurBouton + espace;
    DessinerBoutonMenu(etat, "Enveloppe",  PAGE_ENVELOPPE,  xBouton, y, largeurBouton, hauteurBouton); y += hauteurBouton + espace;
    DessinerBoutonMenu(etat, "Output",     PAGE_OUTPUT,     xBouton, y, largeurBouton, hauteurBouton); y += hauteurBouton + espace;
    DessinerBoutonMenu(etat, "Help",       PAGE_HELP,       xBouton, y, largeurBouton, hauteurBouton); y += hauteurBouton + espace;

    // Bloc moteur audio
    y += 10;
    DrawRectangle(xBouton, y, largeurBouton, 160, (Color){210, 210, 210, 255});
    DrawRectangleLines(xBouton, y, largeurBouton, 160, (Color){160, 160, 160, 255});
    DrawText("MOTEUR AUDIO", xBouton + 15, y + 15, 20, (Color){90, 90, 90, 255});

    Rectangle zoneBoutonAudio = {
        (float)(xBouton + 15),
        (float)(y + 55),
        (float)(largeurBouton - 30),
        80.0f
    };

    Color fond = etat->audioActif ? (Color){60, 170, 90, 255} : (Color){210, 80, 80, 255};
    DrawRectangleRec(zoneBoutonAudio, fond);
    DrawRectangleLinesEx(zoneBoutonAudio, 2, (Color){120, 50, 50, 255});

    if (GuiButton(zoneBoutonAudio, etat->audioActif ? "ON" : "OFF")) {
        etat->audioActif = !etat->audioActif;
    }

    // Restaurer la taille de texte globale
    GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
}

//======================================================================
// Style global Raygui
//======================================================================
void InitGuiStyle(void)
{
    
    float dpi = GetWindowScaleDPI().x;
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(25 * dpi));
}

//======================================================================
// Bouton de forme d'onde : dessine + renvoie true si cliqué
//======================================================================
static bool DessinerBoutonOnde(Rectangle zone, const char *icone,
                               const char *libelle, bool selectionne)
{
    Color couleurFond    = selectionne ? (Color){130, 180, 220, 255} : (Color){200, 200, 200, 255};
    Color couleurBordure = selectionne ? (Color){90, 140, 180, 255}  : (Color){160, 160, 160, 255};

    DrawRectangleRec(zone, couleurFond);
    DrawRectangleLinesEx(zone, 2, couleurBordure);

    int tailleIcone = 24;
    int tailleTexte = 22;

    int largeurIcone = MeasureText(icone, tailleIcone);
    int largeurTexte = MeasureText(libelle, tailleTexte);

    int centreX = (int)(zone.x + zone.width / 2);

    bool clicked = GuiButton(zone, "");

    DrawText(icone,   centreX - largeurIcone / 2, (int)(zone.y + 16), tailleIcone, DARKGRAY);
    DrawText(libelle, centreX - largeurTexte / 2, (int)(zone.y + 46), tailleTexte, DARKGRAY);

    return clicked;
}

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
// Page Oscillateur : bloc "FORME D'ONDE" avec 4 boutons
//======================================================================
static void DessinerPageOscillateur(AppState *etat, int zoneX)
{
    float dpi = GetWindowScaleDPI().x;

    // Largeur disponible
    int margeX = (int)(20 * dpi);
    int x = zoneX + margeX;
    int w = GetScreenWidth() - zoneX - 2*margeX;

    // -----------------------
    // 1) FORME D'ONDE
    // -----------------------
    DrawText("FORME D'ONDE", x, (int)(70*dpi), (int)(20*dpi), DARKGRAY);

    Rectangle zoneForme = { (float)x, (float)(95*dpi), (float)w, (float)(110*dpi) };
    DrawRectangleRec(zoneForme, (Color){220,220,220,255});
    DrawRectangleLinesEx(zoneForme, 2, (Color){170,170,170,255});

    float marge = 14.0f * dpi;
    float espace = 18.0f * dpi;
    float hBtn = 80.0f * dpi;
    float lBtn = (zoneForme.width - 2*marge - 3*espace) / 4.0f;

    Rectangle b1 = { zoneForme.x + marge + (lBtn + espace)*0, zoneForme.y + 15*dpi, lBtn, hBtn };
    Rectangle b2 = { zoneForme.x + marge + (lBtn + espace)*1, zoneForme.y + 15*dpi, lBtn, hBtn };
    Rectangle b3 = { zoneForme.x + marge + (lBtn + espace)*2, zoneForme.y + 15*dpi, lBtn, hBtn };
    Rectangle b4 = { zoneForme.x + marge + (lBtn + espace)*3, zoneForme.y + 15*dpi, lBtn, hBtn };

    if (DessinerBoutonOnde(b1, "~", "Sinusoïdale", etat->formeOnde == ONDE_SINUS))         etat->formeOnde = ONDE_SINUS;
    if (DessinerBoutonOnde(b2, "∏", "Carrée",      etat->formeOnde == ONDE_CARREE))        etat->formeOnde = ONDE_CARREE;
    if (DessinerBoutonOnde(b3, "△", "Triangle",    etat->formeOnde == ONDE_TRIANGULAIRE))  etat->formeOnde = ONDE_TRIANGULAIRE;
    if (DessinerBoutonOnde(b4, "/", "Dent de scie",etat->formeOnde == ONDE_DENT_DE_SCIE))  etat->formeOnde = ONDE_DENT_DE_SCIE;

    // Position Y après formes
    float y = zoneForme.y + zoneForme.height + 20*dpi;

    // -----------------------
    // 2) FREQUENCE
    // -----------------------
    Rectangle groupeFreq = { (float)x, y, (float)w, (float)(110*dpi) };
    DrawRectangleRec(groupeFreq, (Color){220,220,220,255});
    DrawRectangleLinesEx(groupeFreq, 2, (Color){170,170,170,255});

    DessinerSliderAvecBoutons(groupeFreq, "FREQUENCE", &etat->frequenceHz,
                              20.0f, 2000.0f, 10.0f, "%.0f Hz");

    y += groupeFreq.height + 15*dpi;

    // -----------------------
    // 3) VOLUME
    // -----------------------
    Rectangle groupeVol = { (float)x, y, (float)w, (float)(110*dpi) };
    DrawRectangleRec(groupeVol, (Color){220,220,220,255});
    DrawRectangleLinesEx(groupeVol, 2, (Color){170,170,170,255});

    float volumePourcent = etat->volume * 100.0f;
    DessinerSliderAvecBoutons(groupeVol, "VOLUME", &volumePourcent,
                              0.0f, 100.0f, 5.0f, "%.0f %%");
    etat->volume = volumePourcent / 100.0f;

    y += groupeVol.height + 15*dpi;

    // -----------------------
    // 4) PLAY / STOP (gros bouton)
    // -----------------------
    Rectangle btnPlay = { (float)x, y, (float)w, (float)(70*dpi) };

    Color fond = etat->lecture ? (Color){210, 80, 80, 255} : (Color){90, 180, 110, 255};
    DrawRectangleRec(btnPlay, fond);
    DrawRectangleLinesEx(btnPlay, 2, (Color){60,60,60,255});

    const char *txt = etat->lecture ? "■ STOP" : "▶ PLAY";
    int tw = MeasureText(txt, (int)(28*dpi));
    DrawText(txt, (int)(btnPlay.x + btnPlay.width/2 - tw/2), (int)(btnPlay.y + 18*dpi), (int)(28*dpi), RAYWHITE);

    if (CheckCollisionPointRec(GetMousePosition(), btnPlay) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        etat->lecture = !etat->lecture;
    }

    y += btnPlay.height + 15*dpi;

    // -----------------------
    // 5) VISUALISATION (graphique)
    // -----------------------
    DrawText("VISUALISATION", x, (int)(y + 5*dpi), (int)(20*dpi), DARKGRAY);

    Rectangle zoneVisu = { (float)x, y + 35*dpi, (float)w, (float)(220*dpi) };
    DrawRectangleRec(zoneVisu, (Color){30,30,30,255});
    DrawRectangleLinesEx(zoneVisu, 2, (Color){170,170,170,255});

    // Grille légère
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


void DrawAppInterface(AppState *etat)
{
    ClearBackground(RAYWHITE);

    // 1) Menu à gauche
    DessinerMenuLateral(etat);

    // 2) Zone principale à droite
    int zonePrincipaleX = 260;
    DrawRectangle(zonePrincipaleX, 0,
                  GetScreenWidth() - zonePrincipaleX,
                  GetScreenHeight(),
                  (Color){245, 245, 245, 255});

    // 3) Contenu selon la page
    if (etat->pageCourante == PAGE_OSCILLATOR) {
        DessinerPageOscillateur(etat, zonePrincipaleX);
    }
    // plus tard : enveloppe/output/help
}
