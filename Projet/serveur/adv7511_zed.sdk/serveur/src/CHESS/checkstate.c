/*
 * checkstate.cpp
 *
 *  Created on: 2016-11-22
 *      Author: sabark
 */

#include "checkstate.h"

extern Piece player2Pieces[16], player1Pieces[16];

enum State check_king_state(Piece king)
{
	enum moveResult result;
	enum State state = NORMAL;

	Piece *playerPieces;

	if(king.playerID == 1)
		playerPieces = player2Pieces;
	else
		playerPieces = player1Pieces;

	int i = 0;
	for(;  i<16; i++)
	{
		if(playerPieces[i].alive)
		{
			result = execute_move(&playerPieces[i], playerPieces[i].x, king.x, playerPieces[i].y, king.y);
			if(result == VALID)
			{
				state = CHECK;
				return state;
			}
		}
	}

	return state;
}
