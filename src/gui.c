#include "raylib.h"
#include <stdlib.h>


#include "../include/gui_oscillator.h"
#include "../include/gui_output.h"
#include "../include/gui_help.h"

#include "../include/gui_interface.h"
#include "../include/gui_components.h"
#include "../include/gui_oscillator.h"
#include "../include/gui_envelope.h"
#include "../include/gui_piano.h"


#include "../include/raygui.h"

/* ----------------- Utils ----------------- */

float TextToFloat(const char *text)
{
    return strtof(text, NULL);
}

float GetAppDPI(void)
{
    // DPI issues with linux hidpi to be seen later...
    /*#ifdef __linux__
    // On Linux, use DPI scale returned by Raylib if available
    Vector2 scale = GetWindowScaleDPI();
    if (scale.x > 0.0f) return scale.x + (scale.x/2);
    return 1.5f;
    #else*/
    // Default value for other OS
    return 1.5f;
    //#endif
}

/* ------------- Sidebar Menu -------------- */

static void DrawMenuButton(AppState *state, const char *label, PageApp page,
                              int x, int y, int width, int height)
{
    bool active = (state->currentPage == page);

    Color background = active ? (Color){130,180,220,255} : (Color){200,200,200,255};
    Color border = active ? (Color){90,140,180,255}  : (Color){160,160,160,255};

    DrawRectangle(x, y, width, height, background);
    DrawRectangleLines(x, y, width, height, border);

    if (GuiButton((Rectangle){(float)x, (float)y, (float)width, (float)height}, label)) {
        state->currentPage = page;
    }
}

static void DrawSidebar(AppState *state)
{
    float dpi = GetAppDPI();
    int prevTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    int sidebarWidth = (int)(180 * dpi);

    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(14 * dpi));

    // Sidebar background
    DrawRectangle(0, 0, sidebarWidth, GetScreenHeight(), (Color){220,220,220,255});
    DrawText("MENU", (int)(15*dpi), (int)(15*dpi), (int)(18*dpi), DARKGRAY);

    int x = (int)(10*dpi);
    int y = (int)(45*dpi);
    int w = sidebarWidth - (int)(20*dpi);
    int h = (int)(32*dpi);
    int s = (int)(8*dpi);

    DrawMenuButton(state, "#125#Oscillator" /*ICON_WAVE_SINUS*/, PAGE_OSCILLATOR, x, y, w, h); y += h + s;
    DrawMenuButton(state, "#124#Envelope" /*ICON_WAVE*/,  PAGE_ENVELOPE,  x, y, w, h); y += h + s;
    DrawMenuButton(state, "#122#Output" /*ICON_AUDIO*/,     PAGE_OUTPUT,     x, y, w, h); y += h + s;
    DrawMenuButton(state, "#193#Help" /*ICON_HELP*/,       PAGE_HELP,       x, y, w, h); y += h + s;
    DrawMenuButton(state, "#150#Piano", PAGE_PIANO, x, y, w, h); y += h + s;


    // Audio engine block (at bottom)
    y += (int)(20*dpi);

    // separation line
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

    // Color according to state
    Color c = state->audioActive ? (Color){70,180,110,255} : (Color){200,70,70,255};

    // Draw custom button
    DrawRectangleRec(btn, c);
    DrawRectangleLinesEx(btn, 2, (Color){60,60,60,255});

    // Centered text
    const char *txt = state->audioActive ? "AUDIO : ON" : "AUDIO : OFF";
    int fs = (int)(16*dpi);
    int tw = MeasureText(txt, fs);

    DrawText(txt,
            (int)(btn.x + btn.width/2 - tw/2),
            (int)(btn.y + btn.height/2 - fs/2),
            fs,
            RAYWHITE);

    // Clickable zone
    if (CheckCollisionPointRec(GetMousePosition(), btn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        state->audioActive = !state->audioActive;

        // If we stop audio engine => stop playback too
        if (!state->audioActive) {
            state->playback = false;
        }
    }

    // ==========================
    // Playback Mode Button
    // ==========================
    y += (int)(btn.height + 15*dpi);

    DrawText("MODE playback", x + (int)(10*dpi), y, (int)(14*dpi), GRAY);
    y += (int)(20*dpi);

    Rectangle btnMode = {
        (float)(x + 10*dpi),
        (float)y,
        (float)(w - 20*dpi),
        (float)(45*dpi)
    };

    // Color according to mode
    Color cMode = (state->playbackMode == MODE_CONTINUOUS)
                  ? (Color){130,180,220,255}  // Blue (Continuous)
                  : (Color){160,100,200,255}; // Purple (Envelope)

    DrawRectangleRec(btnMode, cMode);
    DrawRectangleLinesEx(btnMode, 2, (Color){60,60,60,255});

    const char *txtMode = (state->playbackMode == MODE_CONTINUOUS) ? "CONTINU" : "ENVELOPPE";
    int twMode = MeasureText(txtMode, fs);

    DrawText(txtMode,
            (int)(btnMode.x + btnMode.width/2 - twMode/2),
            (int)(btnMode.y + btnMode.height/2 - fs/2),
            fs,
            RAYWHITE);

    if (CheckCollisionPointRec(GetMousePosition(), btnMode) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Toggle
        if (state->playbackMode == MODE_CONTINUOUS) state->playbackMode = MODE_ENVELOPE;
        else state->playbackMode = MODE_CONTINUOUS;
    }

GuiSetStyle(DEFAULT, TEXT_SIZE, prevTextSize);
}



void InitGuiStyle(void)
{
    float dpi = GetAppDPI();
    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(16 * dpi));
}

Vector2 BeginPageContent(int zoneX, float *contentHeight, Vector2 *scrollState)
{
    float dpi = GetAppDPI();
    Rectangle panelRec = {
        (float)zoneX,
        0.0f,
        (float)(GetScreenWidth() - zoneX),
        (float)GetScreenHeight()
    };

    float height = *contentHeight;
    if (height < panelRec.height) height = panelRec.height;

    Rectangle contentRec = {
        0.0f,
        0.0f,
        panelRec.width - 20 * dpi,
        height
    };

    Rectangle view = {0};
    GuiScrollPanel(panelRec, NULL, contentRec, scrollState, &view);

    BeginScissorMode((int)view.x, (int)view.y, (int)view.width, (int)view.height);

    return (Vector2){ view.x + scrollState->x, view.y + scrollState->y };
}

void EndPageContent(float finalY, float originY, float *contentHeight)
{
    EndScissorMode();
    *contentHeight = finalY - originY;
}


void DrawAppInterface(AppState *state)
{
    if (state->playback && !state->audioActive) {
    state->audioActive = true;
    }
    ClearBackground(RAYWHITE);

    DrawSidebar(state);

    float dpi = GetAppDPI();
    int sidebarWidth = (int)(180 * dpi);
    int zoneX = sidebarWidth;

    DrawRectangle(zoneX, 0,
                  GetScreenWidth()-zoneX,
                  GetScreenHeight(),
                  (Color){245,245,245,255});

    switch (state->currentPage) {
        case PAGE_OSCILLATOR:
            DrawOscillatorPage(state, zoneX);
            break;
        case PAGE_ENVELOPE:
            DrawEnvelopePage(state, zoneX);
            break;
        case PAGE_OUTPUT:
            DrawOutputPage(state, zoneX);
            break;
        case PAGE_HELP:
            DrawHelpPage(state, zoneX);
            break;
        case PAGE_PIANO:                      
            DrawPianoPage(state, zoneX);   
            break;
        default:
            break;
    }
}
