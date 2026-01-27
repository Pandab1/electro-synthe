#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "gui_components.h"
#include "gui_interface.h"
#include "gui_help.h"
#include "raygui.h"

// Helper function to handle composed labels (e.g. "A / B" or "#12# / #13#")
static void DrawMultiLabel(Rectangle bounds, const char *text) {
    const char *sep = " / ";
    const char *found = strstr(text, sep);

    if (found) {
        // Split detected
        int len1 = (int)(found - text);
        char part1[64] = {0};
        if (len1 < 64) strncpy(part1, text, len1);

        const char *part2 = found + strlen(sep);

        // Divide space into 3 parts (Item1, Separator, Item2)
        float w3 = bounds.width / 3.0f;
        Rectangle r1 = { bounds.x, bounds.y, w3, bounds.height };
        Rectangle r2 = { bounds.x + w3, bounds.y, w3, bounds.height };
        Rectangle r3 = { bounds.x + 2*w3, bounds.y, w3, bounds.height };

        GuiLabel(r1, part1);
        GuiLabel(r2, "/");
        GuiLabel(r3, part2);
    } else {
        // Standard single label
        GuiLabel(bounds, text);
    }
}

static void DrawHelpCard(int x, int y, int w, int h, float dpi, const char *key, const char *desc) {
    Rectangle r = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRec(r, (Color){235, 235, 235, 255});
    DrawRectangleLinesEx(r, 2, (Color){160, 160, 160, 255});

    int keyPadding = (int)(8 * dpi);
    int fontSizeKey = (int)(11 * dpi);
    int keyWidth = MeasureText(key, fontSizeKey) + (int)(16 * dpi);
    if (keyWidth < (int)(60 * dpi)) keyWidth = (int)(60 * dpi);

    // If text contains icons explicitly or is long, ensure enough width
    if (strstr(key, " / ")) keyWidth += (int)(20 * dpi);

    Rectangle keyRect = { (float)x + keyPadding, (float)y + keyPadding, (float)keyWidth, (float)h - (keyPadding * 2) };
    DrawRectangleRec(keyRect, (Color){210, 210, 210, 255});
    DrawRectangleLinesEx(keyRect, 1, (Color){160, 160, 160, 255});

    // Use GuiLabel to support icons like #118#
    int prevAlign = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
    int prevSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSizeKey);

    DrawMultiLabel(keyRect, key);

    GuiSetStyle(LABEL, TEXT_ALIGNMENT, prevAlign);
    GuiSetStyle(DEFAULT, TEXT_SIZE, prevSize);

    int fontSizeDesc = (int)(10 * dpi);
    DrawText(desc, (int)(keyRect.x + keyRect.width + 12 * dpi),
             (int)(y + h/2 - fontSizeDesc/2), fontSizeDesc, GRAY);
}

static void DrawHelpDetailCard(int x, int y, int w, int h, float dpi, const char *title, const char *desc)
{
    Rectangle r = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRec(r, (Color){235,235,235,255});
    DrawRectangleLinesEx(r, 2, (Color){160,160,160,255});

    DrawText(title, x + (int)(12*dpi), y + (int)(10*dpi), (int)(14*dpi), GRAY);
    DrawText(desc, x + (int)(12*dpi), y + (int)(32*dpi), (int)(12*dpi), BLACK);
}

void DrawHelpPage(AppState *state, int zoneX) {
    float dpi = GetAppDPI();

    Vector2 origin = BeginPageContent(zoneX, &state->heightHELP, &state->scrollHELP);

    int marginX = (int)(30 * dpi);
    int usefulWidth = (GetScreenWidth() - zoneX) - (marginX * 2);
    int x = (int)(origin.x + marginX);
    float y = origin.y + 30 * dpi;


    DrawText("AIDE", x, (int)y, (int)(20 * dpi), DARKGRAY);
    y += 45 * dpi;


    DrawText("RACCOURCIS CLAVIER", x, (int)y, (int)(13 * dpi), GRAY);
    y += 25 * dpi;

    int cardH = (int)(40 * dpi);
    int gap = (int)(10 * dpi);
    int colW = (usefulWidth - gap) / 2;

    DrawHelpCard(x, (int)y, colW, cardH, dpi, "ESPACE", "Play / Stop");
    DrawHelpCard(x + colW + gap, (int)y, colW, cardH, dpi, "ECHAP", "Quitter");
    y += cardH + gap;

    char volKeys[32];
    snprintf(volKeys, sizeof(volKeys), "#%d# / #%d#", ICON_ARROW_UP_FILL, ICON_ARROW_DOWN_FILL);
    DrawHelpCard(x, (int)y, colW, cardH, dpi, volKeys, "Volume +/-");

    DrawHelpCard(x + colW + gap, (int)y, colW, cardH, dpi, "+ / -", "Freq +/-");
    y += cardH + gap;

    char arrowKeys[32];
    snprintf(arrowKeys, sizeof(arrowKeys), "#%d# / #%d#", ICON_ARROW_LEFT_FILL, ICON_ARROW_RIGHT_FILL);
    DrawHelpCard(x, (int)y, colW, cardH, dpi, arrowKeys, "Changer d'onde");
    DrawHelpCard(x + colW + gap, (int)y, colW, cardH, dpi, "SHIFT", "Mode playback");
    y += cardH + 35 * dpi;


    DrawText("FORMES D'ONDE", x, (int)y, (int)(13 * dpi), GRAY);
    y += 25 * dpi;

    const char *ondesT[] = {"Sinus", "Carree", "Triangle", "Sawtooth"};
    const char *ondesD[] = {"Onde pure sans harmoniques", "Contient uniquement les harmoniques impaires.", "Harmoniques impaires avec décroissance rapide.", "Contient toutes les harmoniques."};

    int detailH = (int)(60 * dpi);

    for(int i = 0; i < 4; i++) {
        DrawHelpDetailCard(x, (int)y, usefulWidth, detailH, dpi, ondesT[i], ondesD[i]);
        y += detailH + (int)(5 * dpi);
    }
    y += 30 * dpi;


    DrawText("ENVELOPPE ADSR", x, (int)y, (int)(13 * dpi), GRAY);
    y += 25 * dpi;

    const char *adsrT[] = {"Attack", "Decay", "Sustain", "Release"};
    const char *adsrD[] = {"Durée pour passer du silence au volume maximum.", "Durée pour descendre du pic au niveau de sustain.", "Niveau maintenu tant que la note est jouée. ", "Durée pour revenir au silence après le relâchement."};

    for(int i = 0; i < 4; i++) {
        DrawHelpDetailCard(x, (int)y, usefulWidth, detailH, dpi, adsrT[i], adsrD[i]);
        y += detailH + (int)(5 * dpi);
    }
    y += 35 * dpi;

    Rectangle infoBox = { (float)x, y, (float)usefulWidth, (float)(60 * dpi) };
    DrawRectangleRec(infoBox, (Color){230, 240, 250, 255});
    DrawText("raylib - www.raylib.com & raygui - github.com/raysan5/raygui", (int)infoBox.x + 15, (int)infoBox.y + 22, (int)(11 * dpi), DARKGRAY);

    float finalY = y + infoBox.height + 40 * dpi;
    EndPageContent(finalY, origin.y, &state->heightHELP);
}