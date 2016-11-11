#ifndef _BOARD_DISPLAY_H_
#define _BOARD_DISPLAY_H_
#include "chessboard_rest_protocol.h"
#include "xil_types.h"
#define PIECE_DATA_SIZE 800000 // 800k
#define CHARS_DATA_SIZE 200000 // 200k
/******************************************************************************
 * Stuff for the chessboard coordinates and piece identification
******************************************************************************/
typedef enum File {
	A = 0,
	B = 1,
	C = 2,
	D = 3,
	E = 4,
	F = 5,
	G = 6,
	H = 7
} File;

typedef enum Rank {
	R1 = 0,
	R2 = 1,
	R3 = 2,
	R4 = 3,
	R5 = 4,
	R6 = 5,
	R7 = 6,
	R8 = 7
}Rank;

/******************************************************************************
 * PieceType and PieceColor to identify pieces and to find them in the bitmap.
 * PieceType is the horizontal position of the piece and PieceColor is the
 * vertical position.
******************************************************************************/
typedef enum PieceType {
	KING = 0,
	QUEEN = 140,
	ROOK = 300,
	BISHOP = 440,
	KNIGHT = 592,
	PAWN = 750
} PieceType;

typedef enum PieceColor {
	WHITE = 124,
	BLACK = 0
} PieceColor;

/******************************************************************************
 * The data necessary for updating the display after a move has been made.
 * The struct contains the (type, color) of the piece to be moved as well as
 * it's origin (o_file, o_rank) and destination (d_file, d_rank) squares.
******************************************************************************/
struct Move{
	PieceType t;
	PieceColor c;
	File o_file;
	Rank o_rank;
	File d_file;
	Rank d_rank;
	int enPassant;
	int castling;
	int turn_number;
	int capture;
};


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

#endif //_BOARD_DISPLAY_H_


