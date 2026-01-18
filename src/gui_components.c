#include "raylib.h"
#include "../include/raygui.h"   // Include raygui header without implementation
#include "../include/gui_components.h"

void DessinerSliderAvecBoutons(Rectangle zoneGroupe,
                                      const char *titre,
                                      float *valeur,
                                      float min, float max,
                                      float pas,
                                      const char *formatTexte)
{
    float h = zoneGroupe.height;
    // Base reference height is 110.0f. If zone is smaller/bigger, we scale elements.
    float scale = h / 110.0f;
    if (scale < 0.5f) scale = 0.5f; // prevent too small

    int gap = (int)(10 * scale);
    int fontSizeTitle = (int)(16 * scale);
    int fontSizeVal   = (int)(16 * scale);

    DrawText(titre, (int)zoneGroupe.x + gap, (int)zoneGroupe.y + gap * 0.5f, fontSizeTitle, DARKGRAY);

    float btnSize = 32.0f * scale;
    float yControls = zoneGroupe.y + (h * 0.35f);

    // Bouton -
    Rectangle btnMoins = { zoneGroupe.x + gap, yControls, btnSize, btnSize };
    if (GuiButton(btnMoins, "-")) {
        *valeur -= pas;
        if (*valeur < min) *valeur = min;
    }

    // Bouton +
    Rectangle btnPlus = { zoneGroupe.x + zoneGroupe.width - gap - btnSize, yControls, btnSize, btnSize };
    if (GuiButton(btnPlus, "+")) {
        *valeur += pas;
        if (*valeur > max) *valeur = max;
    }

    float sliderH = 16.0f * scale;
    float sliderY = yControls + (btnSize - sliderH)/2.0f;

    Rectangle slider = {
        zoneGroupe.x + gap + btnSize + gap,
        sliderY,
        zoneGroupe.width - 2*(gap + btnSize + gap),
        sliderH
    };
    GuiSlider(slider, "", "", valeur, min, max);

    float yText = zoneGroupe.y + (h * 0.75f);

    const char *txt = TextFormat(formatTexte, *valeur);
    int w = MeasureText(txt, fontSizeVal);
    DrawText(txt, (int)(zoneGroupe.x + zoneGroupe.width/2 - w/2), (int)yText, fontSizeVal, DARKGRAY);

    DrawText(TextFormat(formatTexte, min), (int)(slider.x), (int)yText, (int)(fontSizeVal*0.9f), GRAY);

    const char *txtMax = TextFormat(formatTexte, max);
    int wMax = MeasureText(txtMax, (int)(fontSizeVal*0.9f));
    DrawText(txtMax, (int)(slider.x + slider.width - wMax), (int)yText, (int)(fontSizeVal*0.9f), GRAY);
}

bool DessinerBoutonOnde(Rectangle zone, const char *icone,
                               const char *libelle, bool selectionne)
{
    Color couleurFond    = selectionne ? (Color){130, 180, 220, 255} : (Color){200, 200, 200, 255};
    Color couleurBordure = selectionne ? (Color){90, 140, 180, 255}  : (Color){160, 160, 160, 255};

    DrawRectangleRec(zone, couleurFond);
    DrawRectangleLinesEx(zone, 2, couleurBordure);

    float h = zone.height;
    float scale = h / 80.0f;
    if (scale < 0.3f) scale = 0.3f;

    int tailleIcone = (int)(20 * scale);
    int tailleTexte = (int)(16 * scale);

    int largeurIcone = MeasureText(icone, tailleIcone);
    int largeurTexte = MeasureText(libelle, tailleTexte);

    int centreX = (int)(zone.x + zone.width / 2);

    bool clicked = GuiButton(zone, "");

    DrawText(icone,   centreX - largeurIcone / 2, (int)(zone.y + h * 0.20f), tailleIcone, DARKGRAY);
    DrawText(libelle, centreX - largeurTexte / 2, (int)(zone.y + h * 0.55f), tailleTexte, DARKGRAY);

    return clicked;
}
