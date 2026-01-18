#include "raylib.h"
#include <stdlib.h>


#include "../include/gui_oscillator.h"
#include "../include/gui_envelop.h"
#include "../include/gui_output.h"
#include "../include/gui_help.h"

#include "../include/gui_interface.h"
#include "../include/gui_components.h"

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
    // soucis dpi avec linux hidpi à voir + tard...
    /*#ifdef __linux__
    // Sur Linux, utiliser l'échelle DPI retournée par Raylib si disponible
    Vector2 scale = GetWindowScaleDPI();
    if (scale.x > 0.0f) return scale.x + (scale.x/2);
    return 1.5f;
    #else*/
    // Valeur par défaut pour autres OS
    return 1.5f;
    //#endif
}

/* ------------- Menu latéral -------------- */

static void DessinerBoutonMenu(AppState *etat, const char *libelle, PageApp page,
                              int x, int y, int largeur, int hauteur)
{
    bool actif = (etat->pageCourante == page);

    Color fond = actif ? (Color){130,180,220,255} : (Color){200,200,200,255};
    Color bord = actif ? (Color){90,140,180,255}  : (Color){160,160,160,255};

    DrawRectangle(x, y, largeur, hauteur, fond);
    DrawRectangleLines(x, y, largeur, hauteur, bord);

    if (GuiButton((Rectangle){(float)x, (float)y, (float)largeur, (float)hauteur}, libelle)) {
        etat->pageCourante = page;
    }
}

static void DessinerMenuLateral(AppState *etat)
{
    float dpi = GetAppDPI();
    int prevTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    int largeurMenu = (int)(180 * dpi);

    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(14 * dpi));

    // Fond sidebar
    DrawRectangle(0, 0, largeurMenu, GetScreenHeight(), (Color){220,220,220,255});
    DrawText("MENU", (int)(15*dpi), (int)(15*dpi), (int)(18*dpi), DARKGRAY);

    int x = (int)(10*dpi);
    int y = (int)(45*dpi);
    int w = largeurMenu - (int)(20*dpi);
    int h = (int)(32*dpi);
    int s = (int)(8*dpi);

    DessinerBoutonMenu(etat, "#125#Oscillator" /*ICON_WAVE_SINUS*/, PAGE_OSCILLATOR, x, y, w, h); y += h + s;
    DessinerBoutonMenu(etat, "#124#Enveloppe" /*ICON_WAVE*/,  PAGE_ENVELOPPE,  x, y, w, h); y += h + s;
    DessinerBoutonMenu(etat, "#122#Output" /*ICON_AUDIO*/,     PAGE_OUTPUT,     x, y, w, h); y += h + s;
    DessinerBoutonMenu(etat, "#193#Help" /*ICON_HELP*/,       PAGE_HELP,       x, y, w, h); y += h + s;

    // ==========================
    // Bloc moteur audio (en bas)
    // ==========================
    y += (int)(20*dpi);

    // séparation
    DrawLine(x, y, x + w, y, (Color){160,160,160,255});
    y += (int)(10*dpi);

    DrawText("MOTEUR AUDIO", x + (int)(10*dpi), y, (int)(14*dpi), GRAY);
    y += (int)(20*dpi);

    Rectangle btn = {
        (float)(x + 10*dpi),
        (float)y,
        (float)(w - 20*dpi),
        (float)(45*dpi)
    };

    // Couleur selon état
    Color c = etat->audioActif ? (Color){70,180,110,255} : (Color){200,70,70,255};

    // Dessin du bouton (custom)
    DrawRectangleRec(btn, c);
    DrawRectangleLinesEx(btn, 2, (Color){60,60,60,255});

    // Texte centré
    const char *txt = etat->audioActif ? "AUDIO : ON" : "AUDIO : OFF";
    int fs = (int)(16*dpi);
    int tw = MeasureText(txt, fs);

    DrawText(txt,
            (int)(btn.x + btn.width/2 - tw/2),
            (int)(btn.y + btn.height/2 - fs/2),
            fs,
            RAYWHITE);

    // Zone cliquable (UNE SEULE FOIS)
    if (CheckCollisionPointRec(GetMousePosition(), btn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        etat->audioActif = !etat->audioActif;
    }

        // Si on coupe le moteur audio => on stoppe la lecture aussi
        if (!etat->audioActif) {
            etat->lecture = false;
        }
    }
GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
}


/* ------------- API publique -------------- */

void InitGuiStyle(void)
{
    float dpi = GetAppDPI();
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(16 * dpi));
}



void DrawAppInterface(AppState *etat)
{
    if (etat->lecture && !etat->audioActif) {
    etat->audioActif = true;
    }
    ClearBackground(RAYWHITE);

    DessinerMenuLateral(etat);

    float dpi = GetAppDPI();
    int largeurMenu = (int)(180 * dpi);
    int zoneX = largeurMenu;

    DrawRectangle(zoneX, 0,
                  GetScreenWidth()-zoneX,
                  GetScreenHeight(),
                  (Color){245,245,245,255});

    switch (etat->pageCourante) {
        case PAGE_OSCILLATOR:
            DessinerPageOscillateur(etat, zoneX);
            break;
        case PAGE_ENVELOPPE:
            DessinerPageEnveloppe(etat, zoneX);
            break;
        case PAGE_OUTPUT:
            DessinerPageOutput(etat, zoneX);
            break;
        case PAGE_HELP:
            DessinerPageHelp(etat, zoneX);
            break;
        default:
            break;
    }
}
