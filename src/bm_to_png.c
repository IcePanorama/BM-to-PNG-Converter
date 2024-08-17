#include "raylib.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FILE *load_file (const char *filename);
uint32_t read_uint32_from_file (FILE *fptr);
Color get_color_from_byte (FILE *palette, uint8_t b);
void handle_improper_usage_error (void);

int
main (int argc, char **argv)
{
  if (argc < 3)
    handle_improper_usage_error ();

  const char *bm_filename = argv[1];
  const char *palette_filename = argv[2];

  FILE *palette = load_file (palette_filename);
  FILE *bm_file = load_file (bm_filename);

  uint32_t width = read_uint32_from_file (bm_file);
  uint32_t height = read_uint32_from_file (bm_file);

  // purposely swapping the width & height here
  // this is necessarily for whatever reason
  Image image = GenImageColor (height, width, RAYWHITE);

  fseek (bm_file, 0xC, SEEK_SET);
  for (int x = 0; x < image.width; x++)
    {
      for (int y = 0; y < image.height; y++)
        {
          uint8_t byte;
          size_t bytes_read = fread (&byte, sizeof (uint8_t), 1, bm_file);
          if (bytes_read != 1)
            {
              fprintf (stderr, "fread error, expected %d byte(s), got %zu.\n",
                       1, bytes_read);
              break;
            }

          ImageDrawPixel (&image, x, y, get_color_from_byte (palette, byte));
        }
    }
  fclose (bm_file);
  fclose (palette);

  ImageFormat (&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  ImageFlipVertical (&image);
  ImageRotateCW (&image);

  ExportImage (image, "output.png");

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

Color
get_color_from_byte (FILE *palette, uint8_t b)
{
  fseek (palette, b * 0x3, SEEK_SET);
  uint8_t data[3] = { 0 };
  for (int i = 0; i < 3; i++)
    {
      fread (&data[i], sizeof (uint8_t), 1, palette);
    }
  return (Color){ data[0], data[1], data[2], 255 };
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
