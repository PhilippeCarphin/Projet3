#ifndef _BOARD_DISPLAY_H_
#define _BOARD_DISPLAY_H_
#include "chessboard_rest_protocol.h"
#include "chessboard.h"
#include "xil_types.h"

#define PIECE_DATA_SIZE 800000 // 800k
#define CHARS_DATA_SIZE 200000 // 200k

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
 ******************* Chessboard drawing functions *****************************
******************************************************************************/

/* NOTE: All interface functions send the buffer to the screen over HDMI.*/

/******************************************************************************
 * Displays a fresh board with pieces setup and game information.  Since the 
 * game might not be started yet "White to move" will not be drawn and will
 * only be drawn when BoardDisplay_start_game is called.
******************************************************************************/
int BoardDisplay_new_board(GameInfo *gi);

/******************************************************************************
 * Add additionnal information to the screen after the game has started.
 * For now, it will only draw "White to move".
******************************************************************************/
int BoardDisplay_start_game();

/******************************************************************************
 * Display the welcome screen.
******************************************************************************/
int BoardDisplay_welcome_screen();

/******************************************************************************
 * This function executes a move and writes to the notation rectangle.
******************************************************************************/
int BoardDisplay_move_piece(struct Move *move);

/******************************************************************************
 * Draw pieces in custom positions
******************************************************************************/
int BoardDisplay_draw_pieces_custom(Piece* player1, Piece* player2);

/******************************************************************************
 * Remove the last moved piece and replaces it by another piece on a yellow
 * square. Used when a pawn is promoted.
******************************************************************************/
int BoardDisplay_change_piece_type(PieceType new_type);

#endif //_BOARD_DISPLAY_H_


