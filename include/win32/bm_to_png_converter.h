/**
 *  BM_to_PNG_converter
 *
 *  A header-only library which converts bm files to images (png) using palette
 *  files.
 *
 *  The only non-static function accessible through this header is
 *  `convert_bm_file_to_png ()`. For input, it takes in two `FILE` pointers,
 *  both of which the caller is responsible for initializing and closing. The
 *  first should be a pointer to some bm file, while the second should be a 
 *  pointer to some pal file. The caller is also responsible for insuring that
 *  these are, in fact, bm and pal files. Lastly, the final piece of input
 *  should be the desired output filename. N/B: The function automatically adds
 *  the file extension to the end of `output_filename`.
 *
 *  Additional info on how bm and pal files works:
 *  **BM**:
 *    For whatever reason, these files are organized in a weird fashion. The
 *    first 8 bytes of a given file correspond to its width and height (in
 *    little endian), followed by 4 bytes of zeros. What follows after this is
 *    an array of bytes where each byte corresponds to a pixel in the output
 *    image. The strange part is that this array isn't organized like you might
 *    think. Rather that starting from the upper left hand, it starts from the
 *    bottom left, I believe. I found this out just through trial and error,
 *    but perhaps someone more knowledgable than me knows why. My best guess
 *    is that it's maybe data protection? idk
 *  **PAL**:
 *    These files are very simple compared to the bm files. The "index" you get
 *    from the bm file just tells you where to jump to in the palette and then
 *    you just read the next three bytes which correspond directly to that
 *    pixel's rgb values.
 */
#ifndef _BM_to_PNG_converter_
#define _BM_to_PNG_converter_

#include "raylib.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
              return -1;
            }

          ImageDrawPixel (&image, x, y, get_color_from_byte (pal_file, byte));
        }
    }

  ImageFormat (&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  ImageFlipVertical (&image);
  ImageRotateCW (&image);

  char output[256] = { 0 };
  strcpy (output, output_filename);
  strcat (output, ".png");

  ExportImage (image, output);

  return 0;
}
#endif
