/*
 * checkstate.h
 *
 *  Created on: 2016-11-22
 *      Author: sabark
 */
#ifndef CHECKSTATE_H_
#define CHECKSTATE_H_

#include "chessboard.h"

enum State check_king_state(Piece king);
enum State check_king_move(Piece *king);
enum State king_is_in_check(Piece *king);

#endif /* CHECKSTATE_H_ */
