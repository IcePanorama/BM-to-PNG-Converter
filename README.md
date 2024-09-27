# DEPRECATED

Archived in favor of [BMtoBMP](https://github.com/IcePanorama/BMtoBMP), which converts BM/PAL files to bitmap images instead of PNGs.

This was done primarily to remove the Raylib dependency. At the time I wrote this program, I justified using Raylib as a crutch because the project I created this tool for was going to be using it already. Almost immediately after completing this project though, I regretted the idea. Only just got some time to actually work on this.

The remainder of the old README.md will remain as-is for archival purposes. Note that the [Additional info on how BM and PAL files work](#Additional-info-on-how-BM-and-PAL-files-work) section is rather inaccurate at this point. I learned a lot more about the file format as I worked on this new project. Please view BMtoBMP using the link above if you're looking for more information on these files.

# BM-to-PNG-Converter
A header-only library and command-line executable which converts BM files to images (PNGs) using PAL files.

## Installing

Navigate to the releases page and download the latest zip file.

Once downloaded, extract the zip file wherever you'd like and you're done!

See [usage](#Usage) below for more details on running the application.

### Building from Source (Linux/macOS/Unix)

Install dependencies:

```
# BM-to-PNG dependencies:
gcc make cmake mingw32-gcc clang-tools-extra

# Raylib (v5.0) dependencies:
alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
```

Clone the project:
```bash
$ git clone https://github.com/IcePanorama/BM-to-PNG-Converter.git
# ... or, alternatively ...
$ git clone git@github.com:IcePanorama/BM-to-PNG-Converter.git
```

Install:
```bash
$ cd BM-to-PNG-Converter && mkdir build && cd build && cmake .. && make release && cd ..
```

## Usage
```
BMtoPNG_x86_64.exe path\to\file.BM path\to\file.PAL # 64-bit Windows Systems
BMtoPNG_i686.exe path\to\file.BM path\to\file.PAL # 32-bit Windows Systems
./BMtoPNG path/to/file.BM path/to/file.PAL # Linux/macOS/Unix Systems
```

### Using the library
The only non-static function accessible through the header is `convert_bm_file_to_png ()`. For input, it takes in two `FILE` pointers, both of which the caller is responsible for initializing and closing. The first should be a pointer to some BM file, while the second should be a pointer to some PAL file. The caller is also responsible for insuring that these are, in fact, BM and PAL files. Lastly, the final piece of input should be the desired output filename.

*N/B: The function automatically adds the file extension to the end of `output_filename`.*

## Additional info on how BM and PAL files work:
### BM

For whatever reason, these files are organized in a weird fashion. The first 8 bytes of a given file correspond to its width and height (in little endian), followed by 4 bytes of zeros. What follows after this is an array of bytes where each byte corresponds to a pixel in the output image. The strange part is that this array isn't organized like you might think. Rather that starting from the upper left hand, it starts from the bottom left, I believe. I found this out just through trial and error, but perhaps someone more knowledgable than me knows why. My best guess is that it's maybe data protection? idk

### PAL

These files are very simple compared to the BM files. The "index" you get from the BM file just tells you where to jump to in the PAL file and then you just read the next three bytes which correspond directly to that pixel's rgb values.
