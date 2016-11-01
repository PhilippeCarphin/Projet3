#ifndef _DRAW_HDMI_H_
#define _DRAW_HDMI_H_

#include "bitmap.h"


u32 *get_screen_buffer(void);
u32 get_screen_buffer_length(void);
int set_screen_dimensions(u32 width, u32 height);
int set_background_color(u32 color);
int draw_square(u32 top, u32 left, u32 w, u32 h, u32 color);
int draw_partial_bitmap(u32 screen_top, u32 screen_left,
		u32 img_top, u32 img_left, u32 img_bottom, u32 img_right,
		BMP *bmp, struct RGBA *data);
int draw_full_bitmap(u32 screen_top, u32 screen_left, BMP *bmp, struct RGBA *data);

#endif // _DRAW_HDMI_H_
