#include "raylib.h"
#include <stdio.h>

#include "../include/raygui.h"
#include "../include/gui_interface.h"
#include "../include/gui_components.h"

// Helper: clamp
static float ClampF(float v, float mn, float mx)
{
    if (v < mn) return mn;
    if (v > mx) return mx;
    return v;
}

bool DessinerBoutonOnde(Rectangle zone, int iconId,
                        const char *libelle, bool selectionne)
{
    bool clicked = GuiButton(zone, "");

    if (selectionne) {
        DrawRectangleRec(zone, (Color){130, 180, 220, 255});
        DrawRectangleLinesEx(zone, 2, (Color){90, 140, 180, 255});
    } else {
        DrawRectangleRec(zone, (Color){200, 200, 200, 255});
        DrawRectangleLinesEx(zone, 2, (Color){160, 160, 160, 255});
    }

    int fontSize = (int)(GuiGetStyle(DEFAULT, TEXT_SIZE) * 0.85f);
    int iconSize = fontSize; // Approximate icon size to match font
    int pixelSize = (iconSize / 16) < 1 ? 1 : (iconSize / 16); // Calculate pixel size for GuiDrawIcon

    // icône
    GuiDrawIcon(iconId,
             (int)(zone.x + 8),
             (int)(zone.y + zone.height/2 - (pixelSize*16)/2),
             pixelSize,
             DARKGRAY);

    // label
    DrawText(libelle,
             (int)(zone.x + 30),
             (int)(zone.y + zone.height/2 - fontSize/2),
             fontSize,
             DARKGRAY);

    return clicked;
}


    void DessinerSliderAvecBoutons(Rectangle zoneGroupe,
                                   const char *titre,
                                   float *valeur,
                                   float min, float max,
                                   float pas,
                                   const char *formatTexte){
        float dpi = GetAppDPI();

        int titleSize = (int)(14*dpi);
        int titleX = (int)(zoneGroupe.x + 10*dpi);
        int titleY = (int)(zoneGroupe.y + 6*dpi);

        DrawText(titre, titleX, titleY, titleSize, DARKGRAY);

        char buff[64];
        snprintf(buff, sizeof(buff), formatTexte, *valeur);

        int valueSize = (int)(14*dpi);
        int valueW = MeasureText(buff, valueSize);
        int valueX = (int)(zoneGroupe.x + zoneGroupe.width - 10*dpi - valueW);
        int valueY = titleY;

        DrawText(buff, valueX, valueY, valueSize, DARKGRAY);

        float yControls = zoneGroupe.y + 24*dpi;

        Rectangle btnMoins = {
            zoneGroupe.x + 10*dpi,
            yControls,
            35*dpi,
            22*dpi
        };

        Rectangle btnPlus = {
            zoneGroupe.x + zoneGroupe.width - 10*dpi - 35*dpi,
            yControls,
            35*dpi,
            22*dpi
        };

        if (GuiButton(btnMoins, "-")) *valeur -= pas;
        if (GuiButton(btnPlus,  "+")) *valeur += pas;

        *valeur = ClampF(*valeur, min, max);

        float sliderX = btnMoins.x + btnMoins.width + 10*dpi;
        float sliderW = (btnPlus.x - 10*dpi) - sliderX;

        Rectangle sliderRect = {
            sliderX,
            yControls + 4*dpi,
            sliderW,
            16*dpi
        };

        GuiSlider(sliderRect, "", "", valeur, min, max);

        *valeur = ClampF(*valeur, min, max);
    }

