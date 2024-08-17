#ifndef _BM_to_PNG_converter_
#define _BM_to_PNG_converter_

#include "raylib.h"

#include <stdint.h>
#include <stdio.h>

static uint32_t
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

static Color
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

int8_t
convert_bm_file_to_png (FILE *bm_file, FILE *pal_file,
                        const char *output_filename)
{
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

          ImageDrawPixel (&image, x, y, get_color_from_byte (pal_file, byte));
        }
    }

  ImageFormat (&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  ImageFlipVertical (&image);
  ImageRotateCW (&image);

  ExportImage (image, output_filename);

  return 0;
}
#endif
