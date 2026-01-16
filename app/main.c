#include "custom_math.h"
#include "utils_files.h"
#include "utils_maths.h"
#include <stdint.h>
#include "raylib.h"
#include "../include/gui_interface.h"

typedef int16_t i16;
typedef uint16_t u16;
typedef uint32_t u32;

typedef float f32;

void write_le_16(FILE* f, u16 n){
  fwrite(&n, sizeof(u16), 1, f);
}

void write_le_32(FILE* f, u32 n){
  fwrite(&n, sizeof(u32), 1, f);
}

#define WRITE_STR_LIT(f, s) fwrite ((s), 1, sizeof(s) - 1, f)
#include <stdio.h>

#define FREQ 44100
#define AMPLITUDE 30000

struct Notes {
  f32 freq;
  f32 dur;
};

enum WaveType { SIN, SQU, ST, TRI };

void generate_sound(FILE *f, u32 num_sample, u32 num_notes,
                    struct Notes notes[], enum WaveType type) {
  u32 cur_note = 0;
  f32 cur_note_start = 0.0f;
  for (u32 i = 0; i < num_sample; i++) {
    f32 t = (f32)i / FREQ;

    f32 y = 0.0f;

    if (cur_note < num_notes) {
      switch (type) {
        case SIN:
          y = generate_sin(t, notes[cur_note].freq);
          break;
        case SQU:
          y = generate_square(t, notes[cur_note].freq, AMPLITUDE);
          break;
        case ST:
          y = generate_sawtooth(t, notes[cur_note].freq);
          break;
        case TRI:
          y = generate_triangle(t, notes[cur_note].freq);
          break;
      }

      if (t > cur_note_start + notes[cur_note].dur) {
        cur_note++;
        cur_note_start = t;
      }
    }
    i16 sample = (i16)(y * INT16_MAX);

    write_le_16(f, sample);
  }
}

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;

InitWindow(screenWidth, screenHeight, "Electro-Synthe");
SetTargetFPS(60);

  // Init state
  AppState myState = {
    .showMessage = false,
    .darkMode = false,
    .sliderValue = 50.0f
  };

  InitGuiStyle();

  while (!WindowShouldClose())
  {
    BeginDrawing();
    // gui.c
    DrawAppInterface(&myState);
    EndDrawing();
  }
  CloseWindow();
  return 0;

  FILE *f_sin = fopen("sin.wav", "wb");
  FILE *f_squ = fopen("squ.wav", "wb");
  FILE *f_st = fopen("st.wav", "wb");
  FILE *f_tri = fopen("tri.wav", "wb");

  struct Notes notes[] = {
    {392, 60.0f / 76}, {440, 60.0f / 76}, {294, 60.0f / 114},
    {440, 60.0f / 76}, {494, 60.0f / 76},
};

  u32 num_notes = sizeof(notes) / sizeof(notes[0]);

  f32 duration = 0.0f;
  for (u32 i = 0; i < num_notes; i++) {
    duration += notes[i].dur;
  }

  u32 num_sample = (u32)(duration * FREQ);
  u32 file_size = num_sample * sizeof(u16) + 44;

  create_file_header(f_sin, file_size, FREQ, num_sample);
  create_file_header(f_squ, file_size, FREQ, num_sample);
  create_file_header(f_st, file_size, FREQ, num_sample);
  create_file_header(f_tri, file_size, FREQ, num_sample);

  generate_sound(f_sin, num_sample, num_notes, notes, SIN);
  generate_sound(f_squ, num_sample, num_notes, notes, SQU);
  generate_sound(f_st, num_sample, num_notes, notes, ST);
  generate_sound(f_tri, num_sample, num_notes, notes, TRI);

  fclose(f_sin);
  fclose(f_squ);
  fclose(f_st);
  fclose(f_tri);

  return 0;
}
