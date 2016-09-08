/*
 * SDCardReader.h
 *
 *  Created on: 2016-09-08
 *      Author: frouee && guvacc
 */

#ifndef SDCARDREADER_H_
#define SDCARDREADER_H_

#include <stdio.h>
#include <stdio.h>
#include "ff.h"

int ReadSDCard(char* message,UINT sizeOfMessage,char* path);

#endif /* SDCARDREADER_H_ */
