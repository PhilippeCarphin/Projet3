/*
 * main.c
 *
 *  Created on: 2016-11-01
 *      Author: cllanb
 */

#include "TCP/tcp.h"
#include "HDMI/hdmi.h"

//#include "test_http.h"

int main(){

	init_HDMI();
	init_TCP();

	//test_new_game();
	//test_get_board();

	while(1){
		run_HDMI();
		run_TCP();
	}
	close_HDMI();
	close_TCP();
	return 0;
}
