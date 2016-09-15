/*
 * SDCardReader.c
 *
 *  Created on: 2016-09-08
 *      Author: frouee
 *//******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "SDCardReader.h"


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


