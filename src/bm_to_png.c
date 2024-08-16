#include "raylib.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t read_uint32_from_file (FILE *fptr);
Color get_color_from_byte (FILE *palette, uint8_t b);

int
main (void)
{
  const char *palette_filename = "AUTOGRPH.PAL";
  const char *bm_filename = "AUTOGRPH.BM";

  FILE *palette = fopen (palette_filename, "rb");
  if (palette == NULL)
    {
      fprintf (stderr, "Error opening palette file, %s.\n", palette_filename);
      return -1;
    }

  FILE *bm_file = fopen (bm_filename, "rb");
  if (bm_file == NULL)
    {
      fprintf (stderr, "Error opening bm file, %s.\n", bm_filename);
      return -1;
    }

  uint32_t width = read_uint32_from_file (bm_file);
  uint32_t height = read_uint32_from_file (bm_file);

  // purposely swapping the width & height
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
              fprintf (stderr, "fread error, expected %d bytes, got %zu.\n", 1,
                       bytes_read);
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
