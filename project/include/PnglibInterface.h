#ifndef PNGLIBINTERFACE_H
#define PNGLIBINTERFACE_H

typedef struct
{
    png_structp png_ptr;
    png_infop	info_ptr;
}
PngInfoStruct;

void read_png(FILE* fp, unsigned int sig_read, PngInfoStruct* pinf);
int write_png(const char* file_name, int Width, int Height, void* PixelData);
int write_pngGrayscale(const char* file_name, int Width, int Height, void* PixelData, int ByteProPixel);
void finish_read(PngInfoStruct* pinf);

#endif
