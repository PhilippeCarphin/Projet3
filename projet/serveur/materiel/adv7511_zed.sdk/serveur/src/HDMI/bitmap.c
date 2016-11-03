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

unsigned char dataBucket[MAX_BMP_DATA_SIZE];


static FATFS FatFs;   /* Work area (file system object) for logical drive */
int ReadSDCard(char* message,UINT sizeOfMessage,char* path)
{
    FIL fil;       /* File object */
    FRESULT fr;    /* FatFs return code */


    /* Register work area to the default drive */
    f_mount(&FatFs, "", 0);

    /* Open a text file */
    fr = f_open(&fil, path, FA_READ);
    if (fr) return (int)fr;
    UINT BytesRead;

    int ErrorCode = f_read(&fil,message,sizeOfMessage,&BytesRead );

    /* Close the file */
    f_close(&fil);

    return ErrorCode;
}


void read_4Bytes_LittleEndian(DWORD *dst, unsigned char *src)
{
	*dst = (src[3] << 24) | (src[2] << 16) | (src[1] << 8 ) | src[0];
}

void read_2Bytes_LittleEndian(u16 *dst, unsigned char *src)
{
	*dst = (src[1] << 8 ) | src[0];
}



u32 RGBToNRGB(RGB rgb)
{
	return (1 << 24) + (rgb.Red << 16) + (rgb.Green << 8) + rgb.Blue;
}

u32 RGBA_to_NRGB(struct RGBA rgba)
{
	return (1 << 24) | (rgba.R << 16) | (rgba.G << 8) | rgba.B;
}


int ReadBitmapHeader(BMP *bmp, FIL *fil)
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

void ReadBitMap(BMP *bmp, FIL *fil)
{
	unsigned int bytesRead = 0;
	f_read(fil,dataBucket,bmp->ImageDataSize,&bytesRead);
	xil_printf("%u bytes read:",bytesRead);
}
// if using 1080p
void BitmapToRGB(BMP *bmp, u32* rawHDMIData)
{
	int rgbIndex =0;
	int i = 0;
	int j = 0;
	int widthSkipCounter = 0;
	int heightSkipCounter = 0;
	// we do pixel skipping
	for (i = 0; i < bmp->Height; i++)
	{
		if (++heightSkipCounter == 127)
		{
			heightSkipCounter = 0;
			i+=7;
		}
		for (j = 0; j < bmp->Width; j++)
		{
			if (++widthSkipCounter == 2)
			{
				widthSkipCounter = 0;
				j+=1;
			}
			RGB rgb;
			rgb.Blue = dataBucket[(1920*1080*3)-((i*1920+j)*3)];
			rgb.Green = dataBucket[(1920*1080*3)-((i*1920+j)*3)+1];
			rgb.Red = dataBucket[(1920*1080*3)-((i*1920+j)*3)+2];
			rawHDMIData[rgbIndex] = RGBToNRGB(rgb);
			++rgbIndex;
		}
	}
	xil_printf("%d",rgbIndex);
}

/*******************************************************************************
 * Process data into an array of RGBA and setting the Alpha channel to 0xff for
 * each pixel.  Used in the case of 24 bit images.
*******************************************************************************/
int process_24bit_pixels(BMP *bmp, struct RGBA *dst, int dstMax)
{
	/* Check destination buffer size */
	if( bmp->ImageDataSize / 3 > dstMax)
	{
		xil_printf("%s(): supplied buffer not large enough to hold image: imageData=%d, dstBufferSize=%d.\n",
									__FUNCTION__,bmp->ImageDataSize,dstMax);
		return -1;
	}

	/* Process and transfer data */
	int i = 0;
	int j = 0;
	struct RGBA rgba;
	rgba.A = 0xFF;
	while ( i < bmp->ImageDataSize)
	{
		rgba.B = dataBucket[i++];
		rgba.G = dataBucket[i++];
		rgba.R = dataBucket[i++];

		dst[j++] = rgba;
	}
	return 0;
}

/*******************************************************************************
 * Process 32 bit pixels with alpha channel
*******************************************************************************/
int process_32bit_pixels(BMP *bmp, struct RGBA *dst, int dstMax)
{
	/* Check destination buffer size */
	if( bmp->ImageDataSize / 3 > dstMax)
	{
		xil_printf("%s(): supplied buffer not large enough to hold image: imageData=%d, dstBufferSize=%d.\n",
									__FUNCTION__,bmp->ImageDataSize,dstMax);
		return -1;
	}

	/* Process and transfer data */
	int i = 0;
	int j = 0;
	struct RGBA rgba;
	while ( i < bmp->ImageDataSize)
	{
		rgba.B = dataBucket[i++];
		rgba.G = dataBucket[i++];
		rgba.R = dataBucket[i++];
		rgba.A = dataBucket[i++];
		dst[j++] = rgba;
	}
	return 0;
}

/*******************************************************************************
 * Switch to decide between
*******************************************************************************/
int bitmap_data_to_RGBA_array(BMP *bmp, struct RGBA *dst, int dstMax)
{
	switch (bmp->BitsPerPixel)
	{
	case 24:
		return process_24bit_pixels(bmp,dst,dstMax);
	case 32:
		return process_32bit_pixels(bmp,dst,dstMax);
	default:
		xil_printf("%s():Unsupported bitsPerPixel : %d\n",__FUNCTION__,bmp->BitsPerPixel);
		return -1;
	}
}

void BitMapArrayToNRGBArray(u32* ImgData, RGB* rgbArray, int sizeOfArray)
{
	int i;
	for (i = 0; i < sizeOfArray; i++ )
	{
		ImgData[i] = RGBToNRGB(rgbArray[i]);
	}
}

/*******************************************************************************
 * Interface function.  Client declares
 * 	struct BMP,
 * 	struct RGBA imgData[] // Receptacle to receive data;
 * 	int imgDataMax, // size of buffer to prevent errors;
 * and specifies the path of the file to be read into the memory.
 * EX: In BoardDisplay.c:
 *
 * static struct BMP letters;
 * static struct RGBA letters_data[BLAH];
 * static int letter_width = asdf;
 *
 * static struct BMP pieces;
 * static struct RGBA pieces_data[BLAH2];
 * static int piece_width = asdf; // same as piece height;
 *
 * boardDisplay_init(){
 * 		read_bitmap_file("letters_filename", &letters, &letters_data, MAX_SIZE);
 * 		read_bitmap_file("pieces_filename", &pieces, &pieces_data, MAX_SIZE);
 * }
 *
 * NOTE: mounting of the SD card will have to be donne at the initialization of
 * the application the way that it is done in TP3.
*******************************************************************************/
int read_bitmap_file(char *path, BMP *bmp, struct RGBA *imgData, int imgDataMax)
{
	//ouvrir fichier
	FIL fil;       /* File object */
	FRESULT fr;
	WHERE xil_printf("Path to open = %s\n", path);
    f_mount(&FatFs, "0:/", 1);
	/* Open file */
	fr = f_open(&fil, path, FA_READ);

	if(fr){
		WHERE xil_printf("Unable to open file %s for reading\n", path);
		return -1;
	}

	/* Put info in supplied header struct and read data into dataBucket */
	if(ReadBitmapHeader(bmp,&fil)){
		WHERE xil_printf("Unable to read header for %s\n", path);
		return -1;
	}

	WHERE xil_printf("Header has been read, Height = %d, Width = %d, BitsPerPixel :%d\n", bmp->Height, bmp->Width, bmp->BitsPerPixel);
	ReadBitMap(bmp,&fil);
	/* Process data from dataBucket into destination RGBA array */
	if(bitmap_data_to_RGBA_array(bmp, imgData, imgDataMax))
		return -1;

	// f_close
	return 0;
}

