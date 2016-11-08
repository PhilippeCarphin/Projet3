/*
 * main.c
 *
 *  Created on: 2016-11-01
 *      Author: cllanb
 */

#include "TCP/tcp.h"
#include "HDMI/hdmi.h"
#include "DrawHDMI.h"
#include "BoardDisplay.h"

#include "test_http.h"

int main(){

	// Declare huge buffers on the stack
	u32 screen_buf[1280*1024];
	u8 chars_data[CHARS_DATA_SIZE];
	u8 pieces_data[PIECE_DATA_SIZE];

	// give their addresses to the modules that will use them
	set_screen_buffer(screen_buf);
	BoardDisplay_set_image_buffers(chars_data, pieces_data);

	init_HDMI();
	init_TCP();

	test_http_all();

	while(1){
		run_HDMI();
		run_TCP();
	}
	close_HDMI();
	close_TCP();
	return 0;
}
