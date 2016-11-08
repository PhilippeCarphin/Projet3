/*
 * bitmap.h
 *
 *  Created on: 2016-09-08
 *      Author: frouee && guvacc
 */

#ifndef BITMAP_H_
#define BITMAP_H_
#include <stdio.h>
#include "ff.h"
#include "constant.h"

typedef struct BMP
{
	u16		Magic;				/* Magic identifier: "BM" */
	UINT	FileSize;			/* Size of the BMP file in bytes */
	u16		Reserved1;			/* Reserved */
	u16		Reserved2;			/* Reserved */
	UINT	DataOffset;			/* Offset of image data relative to the file's start */
	UINT	HeaderSize;			/* Size of the header in bytes */
	UINT	Width;				/* Bitmap's width */
	UINT	Height;				/* Bitmap's height */
	u16		Planes;				/* Number of color planes in the bitmap */
	u16		BitsPerPixel;		/* Number of bits per pixel */
	UINT	CompressionType;	/* Compression type */
	UINT	ImageDataSize;		/* Size of uncompressed image's data */
	UINT	HPixelsPerMeter;	/* Horizontal resolution (pixels per meter) */
	UINT	VPixelsPerMeter;	/* Vertical resolution (pixels per meter) */
	UINT	ColorsUsed;			/* Number of color indexes in the color table that are actually used by the bitmap */
	UINT	ColorsRequired;		/* Number of color indexes that are required for displaying the bitmap */
}BMP;


/*******************************************************************************
 * Interface function.  Client declares
 * 	struct BMP,
 * 	struct RGBA imgData[] // Receptacle to receive data;
 * 	int imgDataMax, // size of buffer to prevent errors;
 * and specifies the path of the file to be read into the memory.
 * NOTE: mounting of the SD card will have to be donne at the initialization of
 * the application the way that it is done in TP3.
*******************************************************************************/
int read_bitmap_file(char *path, BMP *bmp, u8 *imgData, int imgDataMax);

/*******************************************************************************
 * Interface function to get the color of a pixel. Returns a u32 whose bytes
 * are AARRGGBB
*******************************************************************************/
u32 get_pixel_rgba(int i, int j, BMP *bmp, u8 *imgData);

int ReadBitmapFile(char* path);

#endif /* BITMAP_H_ */
