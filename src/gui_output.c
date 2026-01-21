#include "raylib.h"
#include "../include/gui_interface.h"
#include "../include/gui_output.h"

#include <math.h>
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

static float DbEstime(float volume01)
{
    if (volume01 < 1e-6f) return -60.0f;
    return 20.0f * log10f(volume01);
}

static void DrawCard(int x, int y, int w, int h, float dpi,
                     const char *label, const char *value, Color valueColor)
{
    Rectangle r = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRec(r, (Color){235,235,235,255});
    DrawRectangleLinesEx(r, 2, (Color){160,160,160,255});

    DrawText(label, x + (int)(12*dpi), y + (int)(10*dpi), (int)(14*dpi), GRAY);
    DrawText(value, x + (int)(12*dpi), y + (int)(34*dpi), (int)(18*dpi), valueColor);
}

static void DrawBar(int x, int y, int w, int h, float dpi, float t01, Color fill)
{
    if (t01 < 0.0f) t01 = 0.0f;
    if (t01 > 1.0f) t01 = 1.0f;

    Rectangle r = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRec(r, (Color){30,30,30,255});
    DrawRectangleLinesEx(r, 2, (Color){120,120,120,255});

    Rectangle rf = { r.x + 2, r.y + 2, (r.width - 4) * t01, r.height - 4 };
    DrawRectangleRec(rf, fill);
}

void DessinerPageOutput(AppState *etat, int zoneX)
{
    float dpi = GetAppDPI();

    Vector2 origin = BeginPageContent(zoneX, &etat->heightOutput, &etat->scrollOutput);

    int mX = (int)(20*dpi);
    int x = (int)(origin.x + mX);
    int startY = (int)(origin.y);
    int y = startY + (int)(30*dpi);
    int w = (int)((GetScreenWidth() - zoneX) - 20 * dpi - 2*mX); // Width based on contentRec

    DrawText("OUTPUT (MONO)", x, y, (int)(20*dpi), DARKGRAY);
    y += (int)(30*dpi);

    int gap = (int)(15*dpi);
    int boxH = (int)(65*dpi);
    int colW = (w - gap) / 2;

    char vForme[64], vFreq[64], vVol[64], vEtat[64];

    snprintf(vForme, sizeof(vForme), "%s", NomForme(etat->formeOnde));
    snprintf(vFreq,  sizeof(vFreq),  "%.0f Hz", etat->frequenceHz);
    snprintf(vVol,   sizeof(vVol),   "%.0f %%", etat->volume * 100.0f);

    const char *playTxt = etat->lecture ? "PLAY" : "STOP";
    const char *audioTxt = etat->audioActif ? "ON" : "OFF";
    snprintf(vEtat, sizeof(vEtat), "%s | AUDIO %s", playTxt, audioTxt);

    DrawCard(x,              y, colW, boxH, dpi, "Forme",     vForme, BLACK);
    DrawCard(x + colW + gap, y, colW, boxH, dpi, "Fréquence", vFreq,  BLACK);

    y += boxH + gap;

    DrawCard(x,              y, colW, boxH, dpi, "Volume",    vVol,   BLACK);

    Color cEtat = etat->audioActif ? (Color){60,170,90,255} : (Color){210,80,80,255};
    DrawCard(x + colW + gap, y, colW, boxH, dpi, "État",      vEtat,  cEtat);

    y += boxH + (int)(25*dpi);

    DrawText("Niveau (estimé)", x, y, (int)(16*dpi), DARKGRAY);
    y += (int)(22*dpi);

    float db = DbEstime(etat->volume);
    char vDb[64];
    snprintf(vDb, sizeof(vDb), "%.1f dB", db);
    DrawText(vDb, x, y, (int)(18*dpi), BLACK);

    y += (int)(28*dpi);

    DrawText("Forme d'onde", rForme.x + 12*dpi, rForme.y + 10*dpi, 14*dpi, GRAY);
    DrawText(NomForme(etat->formeOnde),
             rForme.x + 12*dpi, rForme.y + 34*dpi, 18*dpi, BLACK);
    DrawBar(x, y, w, (int)(18*dpi), dpi, etat->audioActif ? etat->volume : 0.0f,
            etat->audioActif ? (Color){80,180,120,255} : (Color){200,70,70,255});

    char buf[32];
    DrawText("Fréquence", rFreq.x + 12*dpi, rFreq.y + 10*dpi, 14*dpi, GRAY);
    snprintf(buf, sizeof(buf), "%.0f Hz", etat->frequenceHz);
    DrawText(buf, rFreq.x + 12*dpi, rFreq.y + 34*dpi, 18*dpi, BLACK);
    y += (int)(30*dpi);

    DrawText("Volume", rVol.x + 12*dpi, rVol.y + 10*dpi, 14*dpi, GRAY);
    snprintf(buf, sizeof(buf), "%.0f %%", etat->volume * 100.0f);
    DrawText(buf, rVol.x + 12*dpi, rVol.y + 34*dpi, 18*dpi, BLACK);
    DrawText("Format cible : Mono - 44100 Hz - Float32", x, y, (int)(14*dpi), GRAY);
    y += (int)(18*dpi);

    EndPageContent((float)y + 50*dpi, origin.y, &etat->heightOutput);
}
