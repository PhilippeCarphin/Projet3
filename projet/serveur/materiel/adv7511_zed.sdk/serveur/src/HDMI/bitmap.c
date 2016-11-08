/*
 * bitmap.c
 *
 *  Created on: 2016-09-08
 *      Author: frouee && guvacc
 */
#include "bitmap.h"
#include "cf_hdmi.h"
#include <stdlib.h>

#include <stdio.h>
#include "xil_io.h"
#include "debug.h"

const int BMP_HEADER_SIZE = 54;

static FATFS FatFs;   /* Work area (file system object) for logical drive */

/*******************************************************************************
 * Reads 4 bytes from src and forms a u32 to put in dst
*******************************************************************************/
static void read_4Bytes_LittleEndian(DWORD *dst, unsigned char *src)
{
	*dst = (src[3] << 24) | (src[2] << 16) | (src[1] << 8 ) | src[0];
}

/*******************************************************************************
 * Reads 2 bytes from src and forms a u32 to put in dst
*******************************************************************************/
static void read_2Bytes_LittleEndian(u16 *dst, unsigned char *src)
{
	*dst = (src[1] << 8 ) | src[0];
}

/*******************************************************************************
 *
*******************************************************************************/
static int ReadBitmapHeader(BMP *bmp, FIL *fil)
{
	unsigned int bytesRead = 0;
	unsigned char buffer[BMP_HEADER_SIZE];
	unsigned char *current = buffer;
	f_read(fil, buffer, BMP_HEADER_SIZE, &bytesRead);

	if(bytesRead != BMP_HEADER_SIZE )
	{
		return -1;
	}

	read_2Bytes_LittleEndian(&(bmp->Magic), current);current += 2;
	read_4Bytes_LittleEndian(&(bmp->FileSize), current);current += 4;
	read_2Bytes_LittleEndian(&(bmp->Reserved1), current);current += 2;
	read_2Bytes_LittleEndian(&(bmp->Reserved2), current);current += 2;
	read_4Bytes_LittleEndian(&(bmp->DataOffset), current);current += 4;
	read_4Bytes_LittleEndian(&(bmp->HeaderSize), current);current += 4;
	read_4Bytes_LittleEndian(&(bmp->Width), current);current += 4;
	read_4Bytes_LittleEndian(&(bmp->Height), current);current += 4;
	read_2Bytes_LittleEndian(&(bmp->Planes), current);current += 2;
	read_2Bytes_LittleEndian(&(bmp->BitsPerPixel), current);current += 2;
	read_4Bytes_LittleEndian(&(bmp->CompressionType), current);current += 4;
	read_4Bytes_LittleEndian(&(bmp->ImageDataSize), current);current += 4;
	read_4Bytes_LittleEndian(&(bmp->HPixelsPerMeter), current);current += 4;
	read_4Bytes_LittleEndian(&(bmp->VPixelsPerMeter), current);current += 4;
	read_4Bytes_LittleEndian(&(bmp->ColorsUsed), current);current += 4;
	read_4Bytes_LittleEndian(&(bmp->ColorsRequired), current);current += 4;

	return 0;
}

/*******************************************************************************
 * Function to read the bitmap data into the specified buffer.
*******************************************************************************/
static int read_bitmap_data(BMP *bmp, FIL *fil, u8* imgData)
{
	int err = 0;
	unsigned int bytes_read;

	if( (err = f_lseek(fil, bmp->DataOffset)) != 0 ) return err;
	if( (err = f_read(fil, imgData, bmp->ImageDataSize, &bytes_read)) != 0 ) return err;

	return err;

}

/*******************************************************************************
 * Interface function.  Client declares
 * 	struct BMP,
 * 	u8 imgData[] // Receptacle to receive data;
 * 	int imgDataMax, // size of buffer to prevent errors;
 * and specifies the path of the file to be read into the memory.
 * EX: In BoardDisplay.c:
 *
 * NOTE: mounting of the SD card will have to be donne at the initialization of
 * the application the way that it is done in TP3.
*******************************************************************************/
int read_bitmap_file(char *path, BMP *bmp, u8 *imgData, int imgDataMax)
{
	FIL fil;       /* File object */
	FRESULT fr;
	WHERE DBG_PRINT("Path to open = %s\n", path);
    f_mount(&FatFs, "0:/", 1);
	/* Open file */
	fr = f_open(&fil, path, FA_READ);

	if(fr){
		WHERE DBG_PRINT("Unable to open file %s for reading\n", path);
		return -1;
	}

	/* Put info in supplied header struct and read data into dataBucket */
	if(ReadBitmapHeader(bmp,&fil)){
		WHERE DBG_PRINT("Unable to read header for %s\n", path);
		return -1;
	}

	WHERE DBG_PRINT("Header has been read, Height = %d, Width = %d, BitsPerPixel :%d\n", bmp->Height, bmp->Width, bmp->BitsPerPixel);
	read_bitmap_data(bmp,&fil,imgData);

	// f_close
	f_close(&fil);
	return 0;
}

/*******************************************************************************
 * Interface function to get the color of a pixel. Returns a u32 whose bytes
 * are AARRGGBB
*******************************************************************************/
u32 get_pixel_rgba(int i, int j, BMP *bmp, u8 *imgData)
{
	u8 *pixel;
	u32 a=0,r=0,g=0,b=0;
	u32 color;

	u32 bytes_per_row;
	u8 bytes_per_pixel;

	bytes_per_pixel = bmp->BitsPerPixel >> 3;

	bytes_per_row = bmp->ImageDataSize / bmp->Height;

	pixel = imgData + ( (bmp->Height - i - 1) * bytes_per_row + j * bytes_per_pixel );

	a = *(pixel + 0);
	r = *(pixel + 2);
	g = *(pixel + 1);
	b = *(pixel + 3);

	a <<= 24;
	r <<= 16;
	g <<= 8;
	b <<= 0;

	color = (a | r | g | b );

	return color;
}
