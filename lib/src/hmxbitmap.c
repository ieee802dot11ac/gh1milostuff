#include "hmxbitmap.h"
#include "iohelper.h"
#include "hmxstring.h"
#include "hmxcolor.h"
#include <stdio.h>
#include <stdlib.h>

HX_BITMAP hmx_bitmap_load(FILE *file)
{
	HX_BITMAP bmp;
	bmp.version = iohelper_readu8(file);

	bmp.bpp = iohelper_readu8(file);
	bmp.encoding = iohelper_readu32(file);
	if (bmp.encoding != RGBA) {
		fprintf(stderr, "Can only handle RGBA encoding for texture bitmaps currently, sorry!\n");
	}

	bmp.mipmapLevels = iohelper_readu8(file);
	bmp.width = iohelper_readu16(file);
	bmp.height = iohelper_readu16(file);
	bmp.bytesPerLine = iohelper_readu16(file);

	fread(bmp.padding, 1, 19, file);

	bmp.colorPalette = malloc(sizeof(HX_COLOR_8888) * hmx_bitmap_len_color_palette(bmp));
	fread(bmp.colorPalette, sizeof(HX_COLOR_8888), hmx_bitmap_len_color_palette(bmp), file);
	bmp.texData = malloc(sizeof(u8) * hmx_bitmap_len_tex_data(bmp));
	fread(bmp.texData, sizeof(u8), hmx_bitmap_len_tex_data(bmp), file);

	return bmp;
}

void hmx_bitmap_print(HX_BITMAP bmp)
{
	printf("VERSION: %u\n", bmp.version);

	printf("BPP: %u\n", bmp.bpp);
	printf("ENCODING: %s\n", BITMAP_ENCODING_ENUM_name(bmp.encoding));
	printf("WIDTH: %u\n", bmp.width);
	printf("HEIGHT: %u\n", bmp.height);
	printf("BYTES_PER_LINE: %u\n", bmp.bytesPerLine);

	fputs("PADDING: [", stdout);
	for (int i = 0; i < 19; ++i) {
		printf("%u", bmp.padding[i]);
		if (i != 18)
			fputs(", ", stdout);
	}
	puts("]");

	fputs("COLOR PALETTE: [", stdout);
	for (size_t i = 0; i < hmx_bitmap_len_color_palette(bmp); ++i) {
		hmx_color_8888_print(bmp.colorPalette[i]);
		if (i != hmx_bitmap_len_color_palette(bmp) - 1)
			fputs(", ", stdout);
	}
	puts("]");

	puts("TEXTURE DATA: not printing all of this");
}
