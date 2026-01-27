#include "raylib.h"

#include "gui_interface.h"
#include "gui_components.h"
#include "gui_envelope.h"

// Internal function to draw the envelope visualization
static void DrawADSRVisualization(AppState *state, Rectangle zone)
{
    DrawRectangleRec(zone, (Color){20, 20, 20, 255}); // Dark background
    DrawRectangleLinesEx(zone, 2, (Color){170, 170, 170, 255});

    float margin = 20.0f;
    float hUseful = zone.height - (margin * 2);
    float wUseful = zone.width - (margin * 2);

    // Calculate proportions (Total sum for X axis)
    // Normalize relative to maximums: Attack(2s) + Decay(2s) + Sustain(fixed) + Release(3s)
    float totalTime = state->adsr.attack + state->adsr.decay + 1.0f + state->adsr.release;

    // Key points (X, Y)
    Vector2 pStart = { zone.x + margin, zone.y + zone.height - margin };

    // Attack
    Vector2 pAttack = {
        pStart.x + (state->adsr.attack / totalTime) * wUseful,
        zone.y + margin
    };
    
    // Decay
    Vector2 pDecay = {
        pAttack.x + (state->adsr.decay / totalTime) * wUseful,
        pAttack.y + (1.0f - state->adsr.sustain) * hUseful
    };
    
    // Sustain (give it a fixed width for visibility)
    Vector2 pSustainEnd = {
        pDecay.x + (1.0f / totalTime) * wUseful,
        pDecay.y
    };
    
    // Release
    Vector2 pEnd = {
        pSustainEnd.x + (state->adsr.release / totalTime) * wUseful,
        pStart.y
    };

    // Draw envelope lines
    DrawLineV(pStart, pAttack, (Color){130, 180, 220, 255});
    DrawLineV(pAttack, pDecay, (Color){130, 180, 220, 255});
    DrawLineV(pDecay, pSustainEnd, (Color){130, 180, 220, 255});
    DrawLineV(pSustainEnd, pEnd, (Color){130, 180, 220, 255});

    // Labels A, D, S, R under the graph
    int fontSize = 12;
    DrawText("A", (int)pStart.x, (int)(zone.y + zone.height - 15), fontSize, GRAY);
    DrawText("D", (int)pAttack.x, (int)(zone.y + zone.height - 15), fontSize, GRAY);
    DrawText("S", (int)pDecay.x, (int)(zone.y + zone.height - 15), fontSize, GRAY);
    DrawText("R", (int)pSustainEnd.x, (int)(zone.y + zone.height - 15), fontSize, GRAY);
}

void DrawEnvelopePage(AppState *state, int zoneX)
{
    float dpi = GetAppDPI();

    Vector2 origin = BeginPageContent(zoneX, &state->heightEnvelope, &state->scrollEnvelope);

    int marginX = (int)(20 * dpi);
    int x = (int)(origin.x + marginX);
    int w = (int)((GetScreenWidth() - zoneX) - 20 * dpi - 2 * marginX);
    float y = origin.y + 25 * dpi;

    // --- SECTION 1 : EXPLANATION ---
    Rectangle infoZone = { (float)x, y, (float)w, (float)(100 * dpi) };
    DrawRectangleRec(infoZone, (Color){235, 235, 235, 255});
    DrawRectangleLinesEx(infoZone, 1, (Color){180, 180, 180, 255});

    DrawText("ENVELOPPE ADSR", x + 10, (int)(y + 10), (int)(12 * dpi), DARKGRAY);
    DrawText("Attack: Temps pour atteindre le volume max", x + 10, (int)(y + 30), (int)(11 * dpi), GRAY);
    DrawText("Decay: Temps pour descendre au niveau de sustain", x + 10, (int)(y + 50), (int)(11 * dpi), GRAY);
    DrawText("Sustain: Niveau maintenu pendant la note", x + 10, (int)(y + 70), (int)(11 * dpi), GRAY);
    DrawText("Release: Temps pour revenir au silence", x + 10, (int)(y + 90), (int)(11 * dpi), GRAY);

    y += infoZone.height + 15 * dpi;

    // --- SECTION 2 : SLIDERS ---
    float hSliderRec = 70.0f * dpi;
    float spacing = 10.0f * dpi;

    // ATTACK
    Rectangle rAttack = { (float)x, y, (float)w, hSliderRec };
    DrawRectangleRec(rAttack, (Color){230, 230, 230, 255});
    DrawSliderWithButtons(rAttack, "TEMPS D'ATTAQUE", &state->adsr.attack, 0.0f, 2.0f, 0.05f, "%.2fs");
    y += hSliderRec + spacing;

    // DECAY
    Rectangle rDecay = { (float)x, y, (float)w, hSliderRec };
    DrawRectangleRec(rDecay, (Color){230, 230, 230, 255});
    DrawSliderWithButtons(rDecay, "TEMPS DE DECAY", &state->adsr.decay, 0.0f, 2.0f, 0.05f, "%.2fs");
    y += hSliderRec + spacing;

    // SUSTAIN
    Rectangle rSustain = { (float)x, y, (float)w, hSliderRec };
    DrawRectangleRec(rSustain, (Color){230, 230, 230, 255});
    DrawSliderWithButtons(rSustain, "NIVEAU DE SUSTAIN", &state->adsr.sustain, 0.0f, 1.0f, 0.05f, "%.2f");
    y += hSliderRec + spacing;

    // RELEASE
    Rectangle rRelease = { (float)x, y, (float)w, hSliderRec };
    DrawRectangleRec(rRelease, (Color){230, 230, 230, 255});
    DrawSliderWithButtons(rRelease, "TEMPS DE RELEASE", &state->adsr.release, 0.0f, 3.0f, 0.05f, "%.2fs");
    y += hSliderRec + spacing;

    // --- SECTION 3 : VISUALIZATION ---
    DrawText("VISUALISATION ADSR", x, (int)(y + 5), (int)(12 * dpi), DARKGRAY);
    Rectangle visualizationZone = { (float)x, y + 25 * dpi, (float)w, (float)(120 * dpi) };
    DrawADSRVisualization(state, visualizationZone);

    float finalY = visualizationZone.y + visualizationZone.height + 25*dpi; // padding

    EndPageContent(finalY, origin.y, &state->heightEnvelope);
}