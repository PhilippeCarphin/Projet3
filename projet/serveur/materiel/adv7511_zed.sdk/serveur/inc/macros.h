/*
 * macros.h
 *
 *  Created on: 2016-11-01
 *      Author: frouee
 */

#ifndef MACROS_H_
#define MACROS_H_
#include "xil_io.h"

#ifdef DEBUG
#define PRINT(x) xil_printf(x);
#else
#define PRINT(fmt, ...) do{ }while(0);
#endif

#endif /* MACROS_H_ */
