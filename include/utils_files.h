#ifndef FILES_H
#define FILES_H

#include "utils_maths.h"
#include <stdio.h>

#define WRITE_STR_LIT(f, s) fwrite((s), 1, sizeof(s) - 1, f)

void write_le_16(FILE *f, u16 n);

void write_le_32(FILE *f, u32 n);

void create_file_header(FILE *f, u32 filesize, int frequency, u32 num_sample);

#endif // FILES
