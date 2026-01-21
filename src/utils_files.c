#include "utils_files.h"
#include "utils_maths.h"
#include <stdio.h>

void write_le_16(FILE *f, u16 n) { fwrite(&n, sizeof(u16), 1, f); }

void write_le_32(FILE *f, u32 n) { fwrite(&n, sizeof(u32), 1, f); }

void create_file_header(FILE *f, u32 filesize, int frequency, u32 num_sample) {
  WRITE_STR_LIT(f, "RIFF");
  write_le_32(f, filesize - 8);
  WRITE_STR_LIT(f, "WAVE");
  WRITE_STR_LIT(f, "fmt ");
  write_le_32(f, 16);
  write_le_16(f, 1);
  write_le_16(f, 1);
  write_le_32(f, frequency);
  write_le_32(f, frequency * sizeof(u16));
  write_le_16(f, sizeof(u16));
  write_le_16(f, sizeof(u16) * 8);
  WRITE_STR_LIT(f, "data");
  write_le_32(f, num_sample * sizeof(u16));
}
