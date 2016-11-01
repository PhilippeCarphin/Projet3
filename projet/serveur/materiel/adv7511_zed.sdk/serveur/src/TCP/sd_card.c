/*
 * sd_card.c
 *
 *  Created on: 2016-09-20
 *      Author: frouee
 */

#include "ff.h"

static FATFS fat_fs;

void mount_sdcard()
{
	f_mount(&fat_fs, "", 0);
}

