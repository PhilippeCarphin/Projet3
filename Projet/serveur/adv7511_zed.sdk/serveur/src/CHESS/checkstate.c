/*
 * checkstate.cpp
 *
 *  Created on: 2016-11-22
 *      Author: sabark
 */

#include "checkstate.h"

extern Piece player2Pieces[16], player1Pieces[16];
extern Piece* boardGame[8][8];

enum position
{
	x,
	y
};

int piece_can_move_to(Piece *p, int xd, int yd)
{
	return (execute_move(p, p->x, xd, p->y, yd) != ILLEGAL ? 1 : 0);
}

int pieces_can_move_to(Piece *playerPieces,  int xd, int yd)
{
	int i;
	int nb_pieces = 0;
	Piece *p;
	for(i = 0; i<16; i++)
	{
		p = &playerPieces[i];
		if(p->alive && piece_can_move_to(p,xd,yd))
		{
			nb_pieces++;
		}
	}

	return nb_pieces;
}

enum State king_is_in_check(Piece *king)
{
	Piece *opponentPieces;
	if(king->playerID == 1)
		opponentPieces = player2Pieces;
	else
		opponentPieces = player1Pieces;

	if(pieces_can_move_to(opponentPieces,king->x,king->y) != 0)
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
int king_can_move(Piece *king)
{
	enum moveResult result = ILLEGAL;

	Piece *opponentPieces;

	if(king->playerID == 1)
		opponentPieces = player2Pieces;
	else
		opponentPieces = player1Pieces;

	int i = -1;
	int j = -1;

	for(; i<2; i++)
	{
		for(; j<2; j++)
		{
			int xd = king->x +i;
			int yd = king->y +j;

			result = move_king(king->x, xd, king->y, yd);
			if(result == VALID)
			{
				// Verify if he can be in check
				int n = pieces_can_move_to(opponentPieces, xd, yd);
				if(n == 0)
					return 1;

			}
		}
	}

	return 0;
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

/*
 *
 *
 */
int can_castle(Piece *king, Piece *rook, int xd, int yd)
{
	int x_int = (king->x + xd) / 2;
	Piece *opponentPieces;
	if(king->playerID == 1)
		opponentPieces = player2Pieces;
	else
		opponentPieces = player1Pieces;

	if(rook == 0)
		return 0;

	if(king->has_moved)
	{
		return 0;
	}

	if(rook->has_moved)
	{
		return 0;
	}

	if(!rook->alive)
	{
		return 0;
	}

	if(boardGame[x_int][yd] != 0 || boardGame[xd][yd] != 0)
	{
		return 0;
	}

	if(king_is_in_check(king))
	{
		return 0;
	}

	if(pieces_can_move_to(opponentPieces,x_int,yd))
	{
		return 0;
	}

	if(pieces_can_move_to(opponentPieces,xd,yd))
	{
		return 0;
	}

	return 1;
}



