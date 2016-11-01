// IN CF_HDMI.c

#include "bitmap.h"
#include "xil_types.h"

#include "xil_io.h"
#include "DrawHDMI.h"

#define MIN(x, y)	(x < y ? x : y)
#define MAX(x, y) 	(x > y ? x : y)


#define MAX_W 1300
#define MAX_H 1300

#define WHERE xil_printf("%s():",__FUNCTION__);
#define WHEREL xil_printf("%s() (%s,%d)",__FUNCTION__,__FILE__,__LINE__);

#define FBEGIN xil_printf("%s(): Begin\n",__FUNCTION__);
#define FEND xil_printf("%s(): End\n",__FUNCTION__);

#define FONT_BMP_SIZE 300*10000

u32 screenBuf[1280*1024];
/******************************************************************************
* Buffer for the screen.  A long buffer that will be treated as a 2D matrix
* with dimensions w for width and h for height.
******************************************************************************/
struct Screen{
	u32 w;
	u32 h;
	u32 *buffer;
};


/* The screen instance that will be managed by the drawing functions */
static struct Screen screen;

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
		xil_printf("invalid dimensions w = %d, h = %d", width, height);
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
* We are not going to use run-length encoding so each color we set will have
* it's highest byte equal to 1.
* It is important that all entries in the screen have this
******************************************************************************/
int set_pixel(int i, int j, u32 color)
{
	// if the screen is 3 pixels wide, i can be 0,1,2 but not 3 so ">="
	if( j >= screen.w || i >= screen.h)
	{
		WHERE xil_printf("invalid pixel coord w = %d, h = %d, (i,j) = (%d,%d)\n",
													screen.w, screen.h, i,j);
		return -1;
	}

	screen.buffer[i*screen.w + j] = color;

	return 0;
}



/******************************************************************************
* set_pixel function for RGBA pixels.  We deal only with fully transparent
* pixels (A == 0) or fully opaque pixels (A = 0xFF) so either the pixel is
* transparent so we don't draw it in the screen, or the pixel is opaque and
* we replace the current value with a new one.
******************************************************************************/
int set_pixel_RGBA(int i, int j,struct RGBA rgba)
{
	if( rgba.A > 0 )
		screen.buffer[i*screen.w + j] = RGBA_to_NRGB(rgba);

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
		WHERE xil_printf("invalid position of square w = %d, h = %d, (i,j) = (%d,%d)",
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
int draw_partial_bitmap(u32 screen_top, u32 screen_left,
						u32 img_top, u32 img_left, u32 img_bottom, u32 img_right,
						BMP *bmp, struct RGBA *data)
{
	/* Error checks */
	if(screen_top >= screen.h || screen_left >= screen.w)
	{
		WHERE xil_printf("invalid position of square w = %d, h = %d, (i,j) = (%d,%d)",
									 screen.w, screen.h, screen_top,screen_left);
		return -1;
	}
	if(img_top >= bmp->Height    || img_left >= bmp->Width ||
	   img_bottom > bmp->Height  || img_right > bmp->Width ||
	   img_top >= img_bottom     || img_left >= img_right)
	{
		WHERE xil_printf("invalid coordinates in image: (t,l):(%d,%d),  (b,r):(%d,%d)\n", img_top, img_left, img_bottom, img_right);
				return -1;
	}

	int s_i,s_j; // coordinates in the screen
	int b_i, b_j; // coordinates in the bitmap
	for( s_i = screen_top    , b_i = img_top; s_i < screen.h  && b_i < img_bottom; ++s_i, ++b_i)
		for(s_j = screen_left, b_j = img_top; s_j < screen.w  && b_j < img_right ; ++s_j, ++b_j)
		{
			/* Image pixel (b_i,b_j) ---> screen pixel (s_i,s_j)*/
			if(set_pixel_RGBA(s_i,s_j,data[b_i*bmp->Width + b_j])) return -1;
		}
	return 0;
}

int draw_full_bitmap(u32 screen_top, u32 screen_left, BMP *bmp, struct RGBA *data)
{
	return draw_partial_bitmap(screen_top, screen_left, 0,0,bmp->Height, bmp->Width,bmp, data);
}

// SHOOTER ÇA DANS BOARD_DISPLAY
static BMP letters;
static struct RGBA letters_data[FONT_BMP_SIZE];
static u32 letter_width = 10;

int draw_letter(u32 screen_top, u32 screen_left, char c)
{
	return draw_partial_bitmap(screen_top, screen_left,
										0,           letter_width*(c-32),
										letters.Height, letter_width*(c-32+1), &letters, letters_data);
}
