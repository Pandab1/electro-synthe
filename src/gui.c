#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#include "../include/gui_interface.h"
#include "../include/gui_components.h"
#include "../include/gui_oscillator.h"

float TextToFloat(const char *text);

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

// Polyfills for Raylib 5.0 compatibility
float TextToFloat(const char *text)
{
    return strtof(text, NULL);
}

float GetAppDPI(void)
{
    // gestion centralisée du dpi, à voir pour une logique + poussée
    //Vector2 dpi = GetWindowScaleDPI();
    return 1.5f;
}


static void DessinerBoutonMenu(AppState *etat, const char *libelle, PageApp page,
                              int x, int y, int largeur, int hauteur);

static void DessinerMenuLateral(AppState *etat);



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


static void DessinerMenuLateral(AppState *etat)
{
    float dpi = 1.5;
    const int largeurMenu = (int)(180 * dpi);

    int prevTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(14 * dpi));

    DrawRectangle(0, 0, largeurMenu, GetScreenHeight(), (Color){220, 220, 220, 255});
    DrawText("MENU", (int)(15*dpi), (int)(15*dpi), (int)(18*dpi), (Color){70, 70, 70, 255});

    const int xBouton = (int)(10*dpi);
    const int largeurBouton = largeurMenu - (int)(20*dpi);
    const int hauteurBouton = (int)(32*dpi);
    const int espace = (int)(8*dpi);

    int y = (int)(45*dpi);

    DessinerBoutonMenu(etat, "Oscillator", PAGE_OSCILLATOR, xBouton, y, largeurBouton, hauteurBouton); y += hauteurBouton + espace;
    DessinerBoutonMenu(etat, "Enveloppe",  PAGE_ENVELOPPE,  xBouton, y, largeurBouton, hauteurBouton); y += hauteurBouton + espace;
    DessinerBoutonMenu(etat, "Output",     PAGE_OUTPUT,     xBouton, y, largeurBouton, hauteurBouton); y += hauteurBouton + espace;
    DessinerBoutonMenu(etat, "Help",       PAGE_HELP,       xBouton, y, largeurBouton, hauteurBouton); y += hauteurBouton + espace;

    // Bloc moteur audio
    y += (int)(10*dpi);
    int hBlocAudio = (int)(110*dpi);
    DrawRectangle(xBouton, y, largeurBouton, hBlocAudio, (Color){210, 210, 210, 255});
    DrawRectangleLines(xBouton, y, largeurBouton, hBlocAudio, (Color){160, 160, 160, 255});
    DrawText("MOTEUR AUDIO", xBouton + (int)(10*dpi), y + (int)(10*dpi), (int)(14*dpi), (Color){90, 90, 90, 255});

    Rectangle zoneBoutonAudio = {
        (float)(xBouton + 10*dpi),
        (float)(y + 35*dpi),
        (float)(largeurBouton - 20*dpi),
        (float)(60*dpi)
    };

    Color fond = etat->audioActif ? (Color){60, 170, 90, 255} : (Color){210, 80, 80, 255};
    DrawRectangleRec(zoneBoutonAudio, fond);
    DrawRectangleLinesEx(zoneBoutonAudio, 2, (Color){120, 50, 50, 255});

    if (GuiButton(zoneBoutonAudio, etat->audioActif ? "ON" : "OFF")) {
        etat->audioActif = !etat->audioActif;
    }

    GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
}


void InitGuiStyle(void)
{
    float dpi = 1.5;
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(16 * dpi));
}



void DrawAppInterface(AppState *etat)
{
    ClearBackground(RAYWHITE);

    // Sidebar
    DessinerMenuLateral(etat);

    // Viewport principale
    float dpi = GetAppDPI();
    int largeurMenu = (int)(180 * dpi);
    int zonePrincipaleX = largeurMenu;

    DrawRectangle(zonePrincipaleX, 0,
                  GetScreenWidth() - zonePrincipaleX,
                  GetScreenHeight(),
                  (Color){245, 245, 245, 255});

    // Render de la page sélectionnée
    if (etat->pageCourante == PAGE_OSCILLATOR) {
        DessinerPageOscillateur(etat, zonePrincipaleX);
    }
    // TODO: autres pages
}
