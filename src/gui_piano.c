#include "gui_piano.h"
#include "raygui.h"
#include "raylib.h"
#include <math.h>

static float NoteToFreq(int midiNote) {
  return 440.0f * powf(2.0f, (midiNote - 69) / 12.0f);
}

void DrawPianoPage(AppState *state, int zoneX) {
  float dpi = GetAppDPI();
  int x = zoneX + (int)(20 * dpi);
  int y = (int)(20 * dpi);

  DrawText("PIANO", x, y, (int)(24 * dpi), DARKGRAY);
  y += (int)(40 * dpi);

  DrawText("Clavier:", x, y, (int)(18 * dpi), GRAY);
  y += (int)(30 * dpi);

  DrawText("Q W E R T Y U  -> Do Re Mi Fa Sol La Si", x, y, (int)(16 * dpi),
           DARKGRAY);
  y += (int)(25 * dpi);

  DrawText("Clic souris ou clavier sur les touches = joue la note", x, y,
           (int)(16 * dpi), DARKGRAY);
  y += (int)(30 * dpi);

  // Keyboard (C4 -> B4) : 7 white keys
  int keyW = (int)(60 * dpi);
  int keyH = (int)(180 * dpi);

  int kx = x;
  int ky = y;

  const int whiteMidi[7] = {60, 62, 64, 65, 67, 69, 71}; // C4 D4 E4 F4 G4 A4 B4
  const char *whiteLabel[7] = {"Do", "Re", "Mi", "Fa", "Sol", "La", "Si"};
  const char *whiteKeyChar[7] = {"Q", "W", "E", "R", "T", "Y", "U"};
  const int whiteKeyCodes[7] = {KEY_Q, KEY_W, KEY_E, KEY_R,
                                KEY_T, KEY_Y, KEY_U};

  Vector2 mouse = GetMousePosition();
  bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
  bool anyNoteTriggered = false;

  for (int i = 0; i < 7; i++) {
    Rectangle wr = {(float)(kx + i * keyW), (float)ky, (float)keyW,
                    (float)keyH};

    bool hover = CheckCollisionPointRec(mouse, wr);
    bool keyPressed = IsKeyDown(whiteKeyCodes[i]);
    bool active = (hover && mouseDown) || keyPressed;

    if (active) {
      DrawRectangleRec(wr, (Color){200, 200, 200, 255}); // Darken when pressed
    } else {
      DrawRectangleRec(wr, RAYWHITE);
    }

    DrawRectangleLinesEx(wr, 2, (Color){120, 120, 120, 255});
    if (hover && !active)
      DrawRectangleLinesEx(wr, 3, (Color){90, 140, 180, 255});

    // Label note (Do, Re, Mi...)
    int fs1 = (int)(16 * dpi);
    DrawText(whiteLabel[i],
             (int)(wr.x + wr.width / 2 - MeasureText(whiteLabel[i], fs1) / 2),
             (int)(wr.y + wr.height - fs1 - 10), fs1, DARKGRAY);

    // Label touche clavier Q W E R T Y U
    int fs2 = (int)(14 * dpi);
    DrawText(whiteKeyChar[i],
             (int)(wr.x + wr.width / 2 - MeasureText(whiteKeyChar[i], fs2) / 2),
             (int)(wr.y + wr.height - fs1 - fs2 - 16), fs2, GRAY);

    // Clic or Key = play note
    if (active) {
      state->frequencyHz = NoteToFreq(whiteMidi[i]);
      anyNoteTriggered = true;
    }
  }

  if (anyNoteTriggered) {
    state->playback = true;
    state->audioActive = true;
  } else {
    state->playback = false;
  }

  y = ky + keyH + (int)(20 * dpi);
  DrawText(TextFormat("Frequence actuelle: %.2f Hz", state->frequencyHz), x, y,
           (int)(18 * dpi), DARKGRAY);
}
