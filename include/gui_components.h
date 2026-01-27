#ifndef GUI_COMPONENTS_H
#define GUI_COMPONENTS_H
#include "raylib.h"
#include <stdbool.h>

void DrawSliderWithButtons(Rectangle groupZone, const char *title, float *value,
                           float min, float max, float step,
                           const char *textFormat);
bool DrawWaveButton(Rectangle zone, int iconId, const char *label,
                    bool selected);
#endif
