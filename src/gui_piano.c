#include "gui_piano.h"
#include "dsp_oscillator.h"
#include "raygui.h"
#include "raylib.h"
#include <math.h>

// To represent audio output as a visual
void get_visual_buffer(float *dst, int count) {
  unsigned int writeIndex = visWriteIndex;
  for (int i = 0; i < count; i++)
    dst[i] =
        visBuffer[(writeIndex - count + i + VIS_BUFFER_SIZE) % VIS_BUFFER_SIZE];
}

void DrawPianoPage(AppState *state, int zoneX) {
  float dpi = GetAppDPI();
  int x = zoneX + (int)(20 * dpi);
  int y = (int)(20 * dpi);

  DrawText("PIANO", x, y, (int)(24 * dpi), DARKGRAY);
  y += (int)(40 * dpi);

  DrawText("Clavier:", x, y, (int)(18 * dpi), GRAY);
  y += (int)(30 * dpi);

  DrawText("A Z E R T Y U  -> Do Re Mi Fa Sol La Si", x, y, (int)(16 * dpi),
           DARKGRAY);
  y += (int)(25 * dpi);

  // DrawText("Clic souris ou clavier sur les touches = joue la note", x, y,
  //          (int)(16 * dpi), DARKGRAY);
  // y += (int)(30 * dpi);

  // Keyboard (C4 -> B4) : 7 white keys
  int keyW = (int)(60 * dpi);
  int keyH = (int)(180 * dpi);

  int kx = x;
  int ky = y;

  const int whiteMidi[7] = {60, 62, 64, 65, 67, 69, 71}; // C4 D4 E4 F4 G4 A4 B4
  const char *whiteLabel[7] = {"Do", "Re", "Mi", "Fa", "Sol", "La", "Si"};
  const char *whiteKeyChar[7] = {"A", "Z", "E", "R", "T", "Y", "U"};
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

    // // Clic or Key = play note
    // if (active) {
    //   state->frequencyHz = NoteToFreq(whiteMidi[i]);
    //   anyNoteTriggered = true;
    // }
  }

  // if (anyNoteTriggered) {
  //   state->playback = true;
  //   state->audioActive = true;
  // } else {
  //   state->playback = false;
  // }

  float samples[VIS_BUFFER_SIZE];
  get_visual_buffer(samples, VIS_BUFFER_SIZE);

  // Space before
  y = ky + keyH + (int)(20 * dpi);

  // Text
  const char *label = "VISUALISATION";
  int labelFs = (int)(16 * dpi);
  int keyboardWidth = 7 * keyW;
  DrawText(label, kx, y, labelFs, DARKGRAY);

  // Space after
  y += labelFs + (int)(10 * dpi);

  // Oscillo
  int oscWidth = keyboardWidth;
  int oscHeight = (int)(150 * dpi);
  int oscX = kx;
  int oscY = y;

  // Background
  DrawRectangle(oscX, oscY, oscWidth, oscHeight, BLACK);
  DrawRectangleLines(oscX, oscY, oscWidth, oscHeight, DARKGRAY);

  // Waveform
  float gain = 100.0f * dpi;
  int centerY = oscY + oscHeight / 2;

  for (int px = 0; px < oscWidth - 1; px++) {
    int i1 = (px * VIS_BUFFER_SIZE) / oscWidth;
    int i2 = ((px + 1) * VIS_BUFFER_SIZE) / oscWidth;

    int py1 = centerY - (int)(samples[i1] * gain);
    int py2 = centerY - (int)(samples[i2] * gain);

    if (py1 < oscY)
      py1 = oscY;
    if (py1 > oscY + oscHeight)
      py1 = oscY + oscHeight;

    DrawLine(oscX + px, py1, oscX + px + 1, py2, GREEN);
  }
}
