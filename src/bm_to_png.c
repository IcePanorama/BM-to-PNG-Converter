#include "raylib.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t read_uint32_from_file (FILE *fptr);

int
main (void) //(int argc, char **argv)
{
  const char *palette_filename = "AUTOGRPH.PAL";
  const char *bm_filename = "AUTOGRPH.BM";

  FILE *palette = fopen (palette_filename, "rb");
  if (palette == NULL)
    {
      fprintf (stderr, "Error opening palette file, %s.\n", palette_filename);
      return -1;
    }
  fclose (palette);

  FILE *bm_file = fopen (bm_filename, "rb");
  if (bm_file == NULL)
    {
      fprintf (stderr, "Error opening bm file, %s.\n", bm_filename);
      return -1;
    }

  uint32_t width = read_uint32_from_file (bm_file);
  uint32_t height = read_uint32_from_file (bm_file);

  Image image = GenImageColor (width, height, RAYWHITE);

  ImageFormat (&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  ExportImage (image, "Output.png");

  fclose (bm_file);
  return 0;
}

uint32_t
read_uint32_from_file (FILE *fptr)
{
  uint8_t bytes[4];
  size_t bytes_read = fread (bytes, sizeof (uint8_t), 4, fptr);
  if (bytes_read != 4)
    {
      fprintf (stderr, "fread error, read %zu bytes, expected %d.\n",
               bytes_read, 4);
      return -1;
    }
  return ((uint32_t)bytes[3] << 24) | ((uint32_t)bytes[2] << 16)
         | ((uint32_t)bytes[1] << 8) | (uint32_t)bytes[0];
}
