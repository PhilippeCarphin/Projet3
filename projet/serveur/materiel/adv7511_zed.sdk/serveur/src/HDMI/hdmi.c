/***************************************************************************//**
 *   @file   main.c
********************************************************************************
 * Copyright 2013(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
 *   SVN Revision: $WCREV$
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <stdio.h>
#include "xil_cache.h"
#include "xil_types.h"
#include "xil_io.h"
#include "cf_hdmi.h"
#include "atv_platform.h"
#include "transmitter.h"
#include "xil_exception.h"
#include "xuartps.h"
#include "bitmap.h"
#include "drawHDMI.h"
#include "BoardDisplay.h"

extern void delay_ms(u32 ms_count);
extern char inbyte(void);

/******************************************************************************/
/************************** Macros Definitions ********************************/
/******************************************************************************/
#define HDMI_CALL_INTERVAL_MS	10			/* Interval between two         */
											/* iterations of the main loop  */
#define DBG_MSG                 xil_printf

/******************************************************************************/
/************************ Variables Definitions *******************************/
/******************************************************************************/
static UCHAR    MajorRev;      /* Major Release Number */
static UCHAR    MinorRev;      /* Usually used for code-drops */
static UCHAR    RcRev;         /* Release Candidate Number */
static BOOL     DriverEnable;
static BOOL     LastEnable;

static UINT32 StartCount; // previously declared in main(), now initialized in init_HDMI and used in run_HDMI

/***************************************************************************//**
 * @brief Enables the driver.
 *
 * @return Returns ATVERR_OK.
*******************************************************************************/
void APP_EnableDriver (BOOL Enable)
{
    DriverEnable = Enable;
}

/***************************************************************************//**
 * @brief Returns the driver enable status.
 *
 * @return Returns the driver enable status.
*******************************************************************************/
static BOOL APP_DriverEnabled (void)
{
    if ((DriverEnable && HAL_GetMBSwitchState()) != LastEnable)
    {
        LastEnable = DriverEnable && HAL_GetMBSwitchState();
        DBG_MSG ("APP: Driver %s\n\r", LastEnable? "Enabled": "Disabled");
    }
    return (LastEnable);
}

/***************************************************************************//**
 * @brief Displays the application version and the chip revision.
 *
 * @return None.
*******************************************************************************/
static void APP_PrintRevisions (void)
{
	UINT16 TxRev;

	ADIAPI_TxGetChipRevision(&TxRev);

	DBG_MSG("\n\r********************************************************************\r\n");
	DBG_MSG("  ADI HDMI Trasmitter Application Ver R%d.%d.%d\n\r", MajorRev, MinorRev, RcRev);
	DBG_MSG("  HDMI-TX:  ADV7511 Rev 0x%x\r\n", TxRev);
	DBG_MSG("  Created:  %s At %s\n\r", __DATE__, __TIME__);
	DBG_MSG("********************************************************************\r\n\n\r");
}

/***************************************************************************//**
 * @brief Changes the video resolution.
 *
 * @return None.
*******************************************************************************/
static void APP_ChangeResolution (void)
{
	char *resolutions[7] = {"640x480", "800x600", "1024x768", "1280x720", "1360x768", "1600x900", "1920x1080"};
	char receivedChar    = 0;

	if(XUartPs_IsReceiveData(UART_BASEADDR))
	{
		receivedChar = inbyte();
		if((receivedChar >= 0x30) && (receivedChar <= 0x36))
		{
			SetVideoResolution(receivedChar - 0x30);
			DBG_MSG("Resolution was changed to %s \r\n", resolutions[receivedChar - 0x30]);
		}
		else
		{
			if((receivedChar != 0x0A) && (receivedChar != 0x0D))
			{
				SetVideoResolution(RESOLUTION_640x480);
				DBG_MSG("Resolution was changed to %s \r\n", resolutions[0]);
			}
		}
	}
}

/***************************************************************************//**
 * @brief Main function.
 *
 * @return Returns 0.
*******************************************************************************/
int init_HDMI()
{
	MajorRev     = 1;
	MinorRev     = 1;
	RcRev        = 1;
	DriverEnable = TRUE;
	LastEnable   = FALSE;

	Xil_ICacheEnable();
	Xil_DCacheEnable();

#ifdef XPAR_AXI_IIC_0_BASEADDR
	HAL_PlatformInit(XPAR_AXI_IIC_0_BASEADDR,	/* Perform any required platform init */
					 XPAR_SCUTIMER_DEVICE_ID,	/* including hardware reset to HDMI devices */
					 XPAR_SCUGIC_SINGLE_DEVICE_ID,
					 XPAR_SCUTIMER_INTR);
#else
	HAL_PlatformInit(XPAR_AXI_IIC_MAIN_BASEADDR,	/* Perform any required platform init */
					 XPAR_SCUTIMER_DEVICE_ID,	/* including hardware reset to HDMI devices */
					 XPAR_SCUGIC_SINGLE_DEVICE_ID,
					 XPAR_SCUTIMER_INTR);
#endif

	Xil_ExceptionEnable();

#if 0
	//ReadBitmapFile("zedboard.bmp");
	/*
	 * TEST: Attempt to initialize the screen with the right dimensions.
	 *
	 * Use the set the background color to blue,
	 * draw a purple square near the upper right corner.
	 */
	set_screen_dimensions(1280,1024);
	u32 color = (1 << 24) | (0xFF); // Should be blue
	set_background_color(0x00000000); // Set the entire screen to blue.

	u32 sq = 100; // side length of square
	u32 t = 200; // top
	u32 l = 50; // left
	u32 m = 20; // margin

	int i,j;
	// Draw slightly larger square "behind" the chessboard
	draw_square(t-m,l-m,8*sq + 2*m, 8*sq +2*m,(149 << 16) | (92 << 8) | 62);

	// Draw the chessboard
	for (i = 0; i < 8; ++i)
		for(j = 0; j<8; ++j)
		{
			if((i+j)%2 == 0)
				color = 0x00EEEED2;
			else
				color = 0x00769656;
			draw_square(t + i*sq, l + j*sq ,sq ,sq ,color);
		}

	if(0)
	{
		// Tableau pour contenir les donnees
		struct RGBA imageData[64*64];
		// Struct pour recevoir le header
		BMP image;

		// Lecture du bitmap
		// Note, read_bitmap_file ne fais pas de mount de la carte SD
		// static FATFS FatFs;   /* Work area (file system object) for logical drive */
		/* Register work area to the default drive */
		// f_mount(&FatFs, "", 0);
		read_bitmap_file("lepath", &image , imageData, 64*64);
	// Dessiner une partie du bitmap dans l'ecran
	/* int draw_partial_bitmap(u32 screen_top, u32 screen_left,
			u32 img_top, u32 img_left, u32 img_bottom, u32 img_right,
			BMP *bmp, struct RGBA *data);*/
		draw_partial_bitmap(300,300, 0, 0, 123, 456, &image, imageData);
	}
#else
// This replaces the previous block in the #if 0 part.
	set_screen_dimensions(1280,1024);
	set_chess_board_params(200,50,30,10);
	set_background_color(0x00000000); // Set the entire screen to blue.
	draw_chess_board();

	xil_printf("MARKER\n");
#endif

	SetVideoResolution(RESOLUTION_1280x1024);

	InitHdmiAudioPcore();

	APP_PrintRevisions();       /* Display S/W and H/W revisions */

	DBG_MSG("To change the video resolution press:\r\n");
	DBG_MSG("  '0' - 640x480;  '1' - 800x600;  '2' - 1024x768; '3' - 1280x720 \r\n");
	DBG_MSG("  '4' - 1360x768; '5' - 1600x900; '6' - 1920x1080.\r\n");

	ADIAPI_TransmitterInit();   /* Initialize ADI repeater software and h/w */

	ADIAPI_TransmitterSetPowerMode(REP_POWER_UP);

	StartCount = HAL_GetCurrentMsCount();

	return 0;
}

int run_HDMI()
{
	if (ATV_GetElapsedMs (StartCount, NULL) >= HDMI_CALL_INTERVAL_MS)
	{
		StartCount = HAL_GetCurrentMsCount();
		if (APP_DriverEnabled())
		{
			ADIAPI_TransmitterMain();
		}
	}
	APP_ChangeResolution();
	return 0;
}

int close_HDMI()
{
	Xil_DCacheDisable();
	Xil_ICacheDisable();

	return(0);
}
