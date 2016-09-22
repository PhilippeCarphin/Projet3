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


void ReadBitmapHeader(BMP *bmp, FIL *fil)
{
	unsigned int bytesRead = 0;
	unsigned char buffer[BMP_HEADER_SIZE];
	unsigned char *current = buffer;
	f_read(fil, buffer, BMP_HEADER_SIZE, &bytesRead);

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
}

void ReadBitMap(BMP *bmp, FIL *fil)
{
	//bmp->data = (char *) malloc(bmp->ImageDataSize);
	unsigned int bytesRead = 0;
	f_read(fil,dataBucket,bmp->ImageDataSize,&bytesRead);
	xil_printf("%u bytes read:",bytesRead);
}

void BitmapToRGB(BMP *bmp, u32* rawHDMIData)
{
	int rgbIndex =0;
	int bitMapIndex = 0;

	while (bitMapIndex < bmp->ImageDataSize)
	{
		// RGB Color
		//if ((bitMapIndex)%8 != 6)

		{
			RGB rgb;
			rgb.Blue = dataBucket[bitMapIndex++];
			rgb.Green = dataBucket[bitMapIndex++];
			rgb.Red = dataBucket[bitMapIndex++];
			rawHDMIData[rgbIndex] = RGBToNRGB(rgb);
			//rawHDMIData[rgbIndex] = 0x01FF0000;
			++rgbIndex;
		}
		//Padding
		/*else
		{
			bitMapIndex+=2;
		}*/
	}
	xil_printf("%d",rgbIndex);
}

void BitMapArrayToNRGBArray(u32* ImgData, RGB* rgbArray, int sizeOfArray)
{
	int i;
	for (i = 0; i < sizeOfArray; i++ )
	{
		ImgData[i] = RGBToNRGB(rgbArray[i]);
	}
}

void ReadBitmapFile(char* path)
{
	//ouvrir fichier
	FIL fil;       /* File object */
	FRESULT fr;    /* FatFs return code */


	/* Register work area to the default drive */
	f_mount(&FatFs, "", 0);

	/* Open file */
	fr = f_open(&fil, path, FA_READ);
	if (fr) return (int)fr;
	BMP bmp;
	ReadBitmapHeader(&bmp,&fil);
	//bmp.data = malloc ((bmp.FileSize-bmp.HeaderSize) * sizeof(*bmp.data));
	ReadBitMap(&bmp,&fil);
	SetHdmiImageLenght(bmp.Width * bmp.Height);
	xil_printf("%d", bmp.Width);
	BitmapToRGB(&bmp,GetHdmiImagePointer());
	// Make nrgb array from rgbarray
	//store the image file somewhere





}
