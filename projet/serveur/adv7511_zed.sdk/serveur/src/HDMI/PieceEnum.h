/*
 * PieceEnum.h
 *
 *  Created on: 2016-11-11
 *      Author: frouee
 */

#ifndef PIECEENUM_H_
#define PIECEENUM_H_

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
	int enPassant; // Needs to be set by Chessboard.c
	int castling;  // I can figure out if it's castling so chessboard
				   // can just sent KING E1 to G1 and I'll know that it's castling
	int turn_number; // Needs to be set by chessboard.c so that
					 // I can show move numbers on the screen with notation
	int capture; // Needs to be set by chessboard.c so that I can know to write the
				 // 'x' in the chess notation.
};

#endif /* PIECEENUM_H_ */
