#include "raylib.h"
#include "../include/raygui.h"
#include "../include/gui_interface.h"
#include "../include/gui_output.h"

void DessinerPageOutput(AppState *etat, int zoneX)
{
    float dpi = GetAppDPI();
    int x = zoneX + (int)(20*dpi);
    int y = (int)(30*dpi);

    DrawText("PAGE OUTPUT", x, y, (int)(20*dpi), DARKGRAY);
    (void)etat;
}
