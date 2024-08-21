//  Copyright (C) 2024  IcePanorama
//  This file is a part of BMtoPNG.
//  BMtoPNG is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by the
//  Free Software Foundation, either version 3 of the License, or (at your
//  option) any later version.
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//  more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "bm_to_png_converter.h"

#include <stdbool.h>
#include <stdlib.h>

static void handle_improper_usage_error (void);
static void validate_user_input (const char *bm_file,
                                 const char *palette_file);
static FILE *load_file (const char *filename);

int
main (int argc, char **argv)
{
  if (argc < 3)
    handle_improper_usage_error ();
  else
    validate_user_input (argv[1], argv[2]);

  const char *bm_filename = argv[1];
  const char *palette_filename = argv[2];

  FILE *bm_file = load_file (bm_filename);
  FILE *palette = load_file (palette_filename);

  convert_bm_file_to_png (bm_file, palette, "output");

  fclose (bm_file);
  fclose (palette);

  return 0;
}

FILE *
load_file (const char *filename)
{
  FILE *fptr = fopen (filename, "rb");
  if (fptr == NULL)
    {
      fprintf (stderr, "Error opening fptr file, %s.\n", filename);
      exit (1);
    }

  return fptr;
}

void
handle_improper_usage_error (void)
{
  bool windows = false;
#ifdef _WIN32
  windows = true;
#endif
  fprintf (stderr,
           "Improper usage.\n\ttry: %s path/to/file.BM path/to/file.PAL\n",
           windows ? "BMtoPNG_[arch].exe" : "./BMtoPNG");
  exit (1);
}

void
validate_user_input (const char *bm_file, const char *palette_file)
{
  size_t bm_len = strlen (bm_file);
  if (strcmp (bm_file + (bm_len - 3), ".BM") != 0
      && strcmp (bm_file + (bm_len - 3), ".bm") != 0)
    {
      fprintf (stderr, "Error: %s is not a BM file.\n", bm_file);
      handle_improper_usage_error ();
    }

  size_t pal_len = strlen (palette_file);
  if (strcmp (palette_file + (pal_len - 4), ".PAL") != 0
      && strcmp (palette_file + (pal_len - 4), ".pal") != 0)
    {
      fprintf (stderr, "Error: %s is not a PAL file.\n", palette_file);
      handle_improper_usage_error ();
    }
}
