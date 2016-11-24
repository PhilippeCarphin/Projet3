/*
 * checkstate.cpp
 *
 *  Created on: 2016-11-22
 *      Author: sabark
 */

#include "checkstate.h"

extern Piece player2Pieces[16], player1Pieces[16];

enum position
{
	x,
	y
};

int piece_can_move_to(Piece *p, int xd, int yd)
{
	return (execute_move(p, p->x, xd, p->y, yd) != ILLEGAL ? 1 : 0);
}

int a_piece_can_move_to(Piece *playerPieces,  int xd, int yd)
{
	int i;

	Piece *p;
	for(i = 0; i<16; i++)
	{
		p = &playerPieces[i];
		if(p->alive && piece_can_move_to(p,xd,yd))
		{
			return 1;
		}
	}

	return 0;
}

enum State king_is_in_check(Piece *king)
{
	Piece *opponentPieces;
	if(king->playerID == 1)
		opponentPieces = player2Pieces;
	else
		opponentPieces = player1Pieces;

	if(a_piece_can_move_to(opponentPieces,king->x,king->y))
	{
		return CHECK;
	}
	else
	{
		return NORMAL;
	}
}
/*
 * Check the current state of the king
 */
enum State check_king_state(Piece king)
{
	enum moveResult result;
	enum State state = NORMAL;
	int nbThreat = 0;

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
				nbThreat++; // Si le nombre de menaces est superieur a 1, CHECKMATE?
				state = CHECK;

				return state;
			}
		}
	}

	return state;
}

/*
 * Check the state of the king moove
 * 					  |----------|
 * 					  |----------|
 * 					  |---xxx----|
 * 					  |---xrx----|
 */
enum State check_king_move(Piece king)
{
	enum State state = NORMAL;
	enum moveResult result = ILLEGAL;
	int i = -1;
	int j = -1;

	for(; i<2; i++)
	{
		for(; j<2; j++)
		{
			result = move_king(king.x, king.x+i, king.y, king.y+j);
			if(result == VALID)
			{
				// vérifier s'il est en echec dans la nouvelle position
				king.x = king.x+i;
				king.y = king.y+j;
				state = check_king_state(king);
				if(state == NORMAL)
				{
					return state;
				}
			}
		}
	}

	state = CHECKMATE;
	return state;
}

enum position position_to_protect_the_king(Piece threatedPiece, Piece king)
{
	static enum position pos;
	//execute_move(threatedPiece, threatedPiece.x, xPositionToProtect, playerPieces[i].y, yPositionToProtect);

	return pos;
}

/*
 * Check if pieces of the player can save the king
 */
enum moveResult check_moove_pieces_to_protect_king(Piece *playerPieces,  int xPositionToProtect, int yPositionToProtect)
{
	static enum moveResult result = ILLEGAL;
	int i = 0;
	for(; i<16; i++)
	{
		result = execute_move(&playerPieces[i], playerPieces[i].x, xPositionToProtect, playerPieces[i].y, yPositionToProtect);
	}

	return result;
}



