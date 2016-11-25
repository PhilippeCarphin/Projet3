/*
 * checkstate.h
 *
 *  Created on: 2016-11-22
 *      Author: sabark
 */
#ifndef CHECKSTATE_H_
#define CHECKSTATE_H_

#include "chessboard.h"


/******************************************************************************
 * Check if the king is in check
 ******************************************************************************/
enum State king_is_in_check(Piece *king);

/******************************************************************************
 * Check if the king can move to a position without threat
 ******************************************************************************/
int king_can_move(Piece *king);

/******************************************************************************
 * Check if the king can castle
 ******************************************************************************/
int can_castle(Piece *king, Piece *rook, int xd, int yd);

/******************************************************************************
 * Check if we can have a piece that protect the king
 ******************************************************************************/
void protect_king(Piece *king);


#endif /* CHECKSTATE_H_ */
