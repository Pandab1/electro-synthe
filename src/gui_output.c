#include "raylib.h"
#include "../include/raygui.h"
#include "../include/gui_interface.h"
#include "../include/gui_output.h"

#include <stdio.h>

static const char *NomForme(FormeOnde f)
{
    switch (f) {
        case ONDE_SINUS:        return "Sinusoïdale";
        case ONDE_CARREE:       return "Carrée";
        case ONDE_TRIANGULAIRE: return "Triangulaire";
        case ONDE_DENT_DE_SCIE: return "Dent de scie";
        default:                return "Inconnue";
    }
}

void DessinerPageOutput(AppState *etat, int zoneX)
{
    float dpi = GetAppDPI();

    int x = zoneX + 20*dpi;
    int y = 30*dpi;
    int w = GetScreenWidth() - zoneX - 40*dpi;

    DrawText("CONFIGURATION ACTUELLE", x, y, 18*dpi, DARKGRAY);
    y += 25*dpi;

    int boxH = 65*dpi;
    int gap  = 15*dpi;
    int colW = (w - 15*dpi) / 2;

    Rectangle rForme  = { x, y, colW, boxH };
    Rectangle rFreq   = { x + colW + 15*dpi, y, colW, boxH };
    Rectangle rVol    = { x, y + boxH + gap, colW, boxH };
    Rectangle rEtat   = { x + colW + 15*dpi, y + boxH + gap, colW, boxH };

    Rectangle boxes[4] = { rForme, rFreq, rVol, rEtat };

    for (int i = 0; i < 4; i++) {
        DrawRectangleRec(boxes[i], (Color){235,235,235,255});
        DrawRectangleLinesEx(boxes[i], 2, (Color){160,160,160,255});
    }

    DrawText("Forme d'onde", rForme.x + 12*dpi, rForme.y + 10*dpi, 14*dpi, GRAY);
    DrawText(NomForme(etat->formeOnde),
             rForme.x + 12*dpi, rForme.y + 34*dpi, 18*dpi, BLACK);

    char buf[32];
    DrawText("Fréquence", rFreq.x + 12*dpi, rFreq.y + 10*dpi, 14*dpi, GRAY);
    snprintf(buf, sizeof(buf), "%.0f Hz", etat->frequenceHz);
    DrawText(buf, rFreq.x + 12*dpi, rFreq.y + 34*dpi, 18*dpi, BLACK);

    DrawText("Volume", rVol.x + 12*dpi, rVol.y + 10*dpi, 14*dpi, GRAY);
    snprintf(buf, sizeof(buf), "%.0f %%", etat->volume * 100.0f);
    DrawText(buf, rVol.x + 12*dpi, rVol.y + 34*dpi, 18*dpi, BLACK);

    DrawText("État", rEtat.x + 12*dpi, rEtat.y + 10*dpi, 14*dpi, GRAY);
    const char *etatTxt = etat->audioActif ? GuiIconText(ICON_PLAYER_PLAY, "EN LECTURE") : GuiIconText(ICON_PLAYER_STOP, "ARRÊTÉ");
    Color c = etat->audioActif ? (Color){60,170,90,255} : (Color){210,80,80,255};

    int prevColor = GuiGetStyle(LABEL, TEXT_COLOR_NORMAL);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(c));
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

    GuiLabel((Rectangle){rEtat.x + 10*dpi, rEtat.y + 34*dpi, rEtat.width, 24*dpi}, etatTxt);

    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, prevColor);
}
