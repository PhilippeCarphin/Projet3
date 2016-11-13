#ifndef _BOARD_DISPLAY_H_
#define _BOARD_DISPLAY_H_
#include "chessboard_rest_protocol.h"
#include "chessboard.h"
#include "xil_types.h"

#define PIECE_DATA_SIZE 800000 // 800k
#define CHARS_DATA_SIZE 200000 // 200k




// TEMPORARY:
struct PlayerTimes{
	char player1[10];
	char player2[10];
};

/******************************************************************************
 * Initialization functions:
 *  - The first thing is to call BoardDisplay_set_image_buffers() to give
 *    the module memory to store the bitmap data for it's chess pieces and
 *    letters.
 *  - Once this is done, BoardDisplay_init will load image data in the buffers
 *    that were set using the previous function.
******************************************************************************/
int BoardDisplay_set_image_buffers(u8 *chars_dat, u8 *pieces_dat);
int BoardDisplay_init();

/******************************************************************************
 * Chessboard drawing functions
 * - draw_chess_board() draws the chessboard with pieces in their initial
 *   positions.
 * - BoardDisplay_move_piece() moves the piece using data specified in the
 *   move data structure.
 * - draw_information() will receive the gameinfo struct and will draw some
 *   of the contained information on the screen.
******************************************************************************/
int draw_chess_board();
int BoardDisplay_move_piece(struct Move *move);
int draw_information(struct GameInfo *gi);
int update_times(struct PlayerTimes *pt);
int BoardDisplay_new_board(GameInfo *gi);
int BoardDisplay_draw_pieces_custom(Piece* player1, Piece* player2);
int BoardDisplay_welcome_screen();

/******************************************************************************
 * Function that tests the workings of the whole HDMI module and is usable as
 * documentation for how to use the BoardDisplay_move_piece function.
******************************************************************************/
int test_move_piece();

#endif //_BOARD_DISPLAY_H_


