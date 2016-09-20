/*
 * bitmap.c
 *
 *  Created on: 2016-09-08
 *      Author: frouee && guvacc
 */
#include "bitmap.h"
#include <stdlib.h>

#include <stdio.h>
#include "xil_io.h"






void ReadBitMap(char* bitMap, RGB* rgbOutArray)
{
	int size = bitMap[2] + (bitMap[3] << 8) + (bitMap[4] << 16) + (bitMap[5] << 24);
	int offset = bitMap[10] + (bitMap[11] << 8) + (bitMap[12] << 16) + (bitMap[13] << 24);
	width = bitMap[18] + (bitMap[19] << 8) + (bitMap[20] << 16) + (bitMap[21] << 24);
	heigth = bitMap[22] + (bitMap[23] << 8) + (bitMap[24] << 16) + (bitMap[25] << 24);
	rgbOutArray = (RGB *) malloc(sizeof(*rgbOutArray) * width*heigth);
	int rgbIndex =0;
	int bitMapIndex =offset;

	while (bitMapIndex < size)
	{
		// RGB Color
		if ((bitMapIndex-offset)%8 != 6 && (bitMapIndex - offset)%8 != 7)
		{
			rgbOutArray[rgbIndex].Blue = bitMap[bitMapIndex++];
			rgbOutArray[rgbIndex].Green = bitMap[bitMapIndex++];
			rgbOutArray[rgbIndex].Red = bitMap[bitMapIndex++];
			++rgbIndex;
		}
		// Padding
		else
		{
			bitMapIndex+=2;
		}
	}
}


u32 RGBToNRGB(RGB rgb)
{
	return (1 << 24) + (rgb.Red << 16) + (rgb.Green << 8) + rgb.Blue;
}

void BitMapArrayToNRGBArray(u32* ImgData, RGB* rgbArray, int sizeOfArray)
{
	ImgData = malloc(sizeof(u32)*sizeOfArray);
	int i;
	for (i = 0; i < sizeOfArray; i++ )
	{
		ImgData[i] = RGBToNRGB(rgbArray[i]);
	}
}
