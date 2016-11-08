// IN CF_HDMI.c

#include "bitmap.h"
#include "xil_types.h"

#include "xil_io.h"
#include "DrawHDMI.h"
#include "debug.h"

#define MIN(x, y)	(x < y ? x : y)
#define MAX(x, y) 	(x > y ? x : y)


#define MAX_W 1300
#define MAX_H 1300




/* The screen instance that will be managed by the drawing functions */
struct Screen screen;

int set_screen_buffer(u32 *screen_buffer)
{
	screen.buffer = screen_buffer;
	return 0;
}

/******************************************************************************
* Get the address of the screen buffer.  This will be used to give to the
* cf_hdmi module for outputting to the screen.
******************************************************************************/
u32 *get_screen_buffer(void)
{
	return screen.buffer;
}

/******************************************************************************
* Get the buffer length in number of pixels.
******************************************************************************/
u32 get_screen_buffer_length(void)
{
	return screen.w * screen.h;
}

/******************************************************************************
* Sets the whole screen to the specified color.
******************************************************************************/
int set_background_color(u32 color)
{
	u32 length = screen.w * screen.h;
	int i = 0;
	for (i = 0; i < length; ++i)
	{
		screen.buffer[i] = color;
	}
	return 0;
}

/******************************************************************************
* Function to set dimensions of the screen with a check that we are not using
* dimensions that will overflow the buffer.
******************************************************************************/
int set_screen_dimensions(u32 width, u32 height)
{
	FBEGIN
	if( width > MAX_W || height > MAX_H)
	{
		DBG_PRINT("invalid dimensions w = %d, h = %d", width, height);
		return -1;
	}

	screen.w = width;
	screen.h = height;

	FEND
	return 0;
}

/******************************************************************************
* Function to manage the screen as a 2D matrix.  Setting pixel (i,j) to the
* specified color.  Contains a check for dimensions of the screen.
*
* We're not doing run-length-encoding, so the position of the pixel in the
* buffer corresponds exactly to a pixel in the screen.
******************************************************************************/
int set_pixel(int i, int j, u32 color)
{
	// if the screen is 3 pixels wide, i can be 0,1,2 but not 3 so ">="
	if( j >= screen.w || i >= screen.h)
	{
		WHERE DBG_PRINT("invalid pixel coord w = %d, h = %d, (i,j) = (%d,%d)\n",
													screen.w, screen.h, i,j);
		return -1;
	}

	screen.buffer[i*screen.w + j] = color;

	return 0;
}

/******************************************************************************
* Function to manage the screen as a 2D matrix.  Setting pixel (i,j) to the
* specified color.  Contains a check for dimensions of the screen.
*
* We are not going to use run-length encoding so each color we set will have
* it's highest byte equal to 1.
* It is important that all entries in the screen have this
******************************************************************************/
int set_pixel_rgba(int i, int j, u32 color)
{
	if( j >= screen.w || i >= screen.h)
	{
		WHERE DBG_PRINT("invalid pixel coord w = %d, h = %d, (i,j) = (%d,%d)\n",
													screen.w, screen.h, i,j);
		return -1;
	}

	if( color & 0xFF000000 )
		screen.buffer[i*screen.w + j] = color & 0x00ffffff;

	return 0;
}

/*******************************************************************************
 * This function draws a square of dimensions w x h with the specified color.
 * The position of the square is given by giving the position of the top-left
 * corner.
 * Note that the function checks the screen dimensions when deciding the
 * bottom-right corner.
 *
 * The function takes care of verifying that the boundaries of the screen are
 * not violated.
*******************************************************************************/
int draw_square(u32 top, u32 left, u32 w, u32 h, u32 color)
{
	FBEGIN
	if(top >= screen.h || left >= screen.w)
	{
		WHERE DBG_PRINT("invalid position of square w = %d, h = %d, (i,j) = (%d,%d)",
									 screen.w, screen.h, top,left);
		return -1;
	}
	int bottom = MIN(top + h, screen.h);
	int right = MIN(left + w, screen.w);

	int i,j;
	for(i = top; i < bottom; ++i)
	{
		for(j = left; j < right; ++j)
		{
			if(set_pixel(i,j,color)) return -1;
		}
	}
	FEND
	return 0;
}

/******************************************************************************
 * For the bitmap specified by the pair (bmp, data), draw the part of said bitmap
 * delimited by (img_top, img_left), (img_bottom, img_right) in the screen,
 * placing (img_top, img_left) at (screen_top, screen_left) in the screen.
 * (img_bottom, img_left) not included so that to draw the whole image, we can do
 * (0,0) and (bmp->Height, bmp->Width)
******************************************************************************/
int draw_partial_bitmap(  u32 screen_top, u32 screen_left,
							u32 img_top, u32 img_left, u32 img_bottom, u32 img_right,
							BMP *bmp, u8 *data)
{
	/*
	 * Error checks
	 */
	if(screen_top >= screen.h || screen_left >= screen.w)
	{
		WHERE DBG_PRINT("invalid position of square w = %d, h = %d, (i,j) = (%d,%d)",
									 screen.w, screen.h, screen_top,screen_left);
		return -1;
	}
	if(img_top >= bmp->Height    || img_left >= bmp->Width ||
	   img_bottom > bmp->Height  || img_right > bmp->Width ||
	   img_top >= img_bottom     || img_left >= img_right)
	{
		WHERE DBG_PRINT("invalid coordinates in image: (t,l):(%d,%d),  (b,r):(%d,%d)\n", img_top, img_left, img_bottom, img_right);
				return -1;
	}

	/*
	 * Drawing in the screen
	 */
	int s_i,s_j; // coordinates in the screen
	int b_i, b_j; // coordinates in the bitmap
	for( s_i = screen_top    , b_i = img_top; s_i < screen.h  && b_i < img_bottom; ++s_i, ++b_i)
	{
		for(s_j = screen_left, b_j = img_left; s_j < screen.w  && b_j < img_right ; ++s_j, ++b_j)
		{
			/* Image pixel (b_i,b_j) ---> screen pixel (s_i,s_j)*/
			if(set_pixel_rgba(s_i,s_j,get_pixel_rgba(b_i,b_j,bmp,data))) return -1;
		}
	}
	return 0;
}

int draw_full_bitmap(u32 screen_top, u32 screen_left, BMP *bmp, u8 *data)
{
	return draw_partial_bitmap(screen_top, screen_left, 0,0,bmp->Height, bmp->Width,bmp, data);
}
