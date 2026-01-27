#include "raylib.h"
#include <stdio.h>

#include "gui_components.h"
#include "gui_interface.h"
#include "raygui.h"

// Helper: clamp
static float ClampF(float v, float mn, float mx) {
  if (v < mn)
    return mn;
  if (v > mx)
    return mx;
  return v;
}

bool DrawWaveButton(Rectangle zone, int iconId, const char *label,
                    bool selected) {
  bool clicked = GuiButton(zone, "");

  if (selected) {
    DrawRectangleRec(zone, (Color){130, 180, 220, 255});
    DrawRectangleLinesEx(zone, 2, (Color){90, 140, 180, 255});
  } else {
    DrawRectangleRec(zone, (Color){200, 200, 200, 255});
    DrawRectangleLinesEx(zone, 2, (Color){160, 160, 160, 255});
  }

  int fontSize = (int)(GuiGetStyle(DEFAULT, TEXT_SIZE) * 0.85f);
  int iconSize = fontSize; // Approximate icon size to match font
  int pixelSize = (iconSize / 16) < 1
                      ? 1
                      : (iconSize / 16); // Calculate pixel size for GuiDrawIcon

  // icon
  GuiDrawIcon(iconId, (int)(zone.x + 8),
              (int)(zone.y + zone.height / 2 - (pixelSize * 16) / 2), pixelSize,
              DARKGRAY);

  // label
  DrawText(label, (int)(zone.x + 30),
           (int)(zone.y + zone.height / 2 - fontSize / 2), fontSize, DARKGRAY);

  return clicked;
}

void DrawSliderWithButtons(Rectangle groupZone, const char *title, float *value,
                           float min, float max, float step,
                           const char *textFormat) {
  float dpi = GetAppDPI();

  int titleSize = (int)(14 * dpi);
  int titleX = (int)(groupZone.x + 10 * dpi);
  int titleY = (int)(groupZone.y + 6 * dpi);

  DrawText(title, titleX, titleY, titleSize, DARKGRAY);

  char buff[64];
  snprintf(buff, sizeof(buff), textFormat, *value);

  int valueSize = (int)(14 * dpi);
  int valueW = MeasureText(buff, valueSize);
  int valueX = (int)(groupZone.x + groupZone.width - 10 * dpi - valueW);
  int valueY = titleY;

  DrawText(buff, valueX, valueY, valueSize, DARKGRAY);

  float yControls = groupZone.y + 24 * dpi;

  Rectangle btnMinus = {groupZone.x + 10 * dpi, yControls, 35 * dpi, 22 * dpi};

  Rectangle btnPlus = {groupZone.x + groupZone.width - 10 * dpi - 35 * dpi,
                       yControls, 35 * dpi, 22 * dpi};

  if (GuiButton(btnMinus, "-"))
    *value -= step;
  if (GuiButton(btnPlus, "+"))
    *value += step;

  *value = ClampF(*value, min, max);

  float sliderX = btnMinus.x + btnMinus.width + 10 * dpi;
  float sliderW = (btnPlus.x - 10 * dpi) - sliderX;

  Rectangle sliderRect = {sliderX, yControls + 4 * dpi, sliderW, 16 * dpi};

  GuiSlider(sliderRect, "", "", value, min, max);

  *value = ClampF(*value, min, max);
}
