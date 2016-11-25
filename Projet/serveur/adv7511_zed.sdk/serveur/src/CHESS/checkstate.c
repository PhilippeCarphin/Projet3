/*
 * checkstate.cpp
 *
 *  Created on: 2016-11-22
 *      Author: sabark
 */

#include "checkstate.h"

extern Piece player2Pieces[16], player1Pieces[16];
extern Piece* boardGame[8][8];
extern TurnInfo currentTurnInfo;

/******************************************************************************
 * Declarations of internal functions
 ******************************************************************************/
static void protect_king_from_vertical_attack(Piece *king , Piece *threatPiece);
static void protect_king_from_diagonal_attack(Piece *king , Piece *threatPiece);
static Piece *get_own_pieces(Piece *p);
static Piece *get_opponent_pieces(Piece *p);
static int piece_can_move_to(Piece *p, int xd, int yd);
static int pieces_can_move_to(Piece *playerPieces,  int xd, int yd);
static Piece* piece_can_move(Piece *playerPieces,  int xd, int yd);

/******************************************************************************
 * Check if the king is in check
 ******************************************************************************/
enum State king_is_in_check(Piece *king)
{
	Piece *opponentPieces = get_opponent_pieces(king);;

	if(pieces_can_move_to(opponentPieces,king->x,king->y) != 0)
	{
		return CHECK;
	}
	else
	{
		return NORMAL;
	}
}

/******************************************************************************
 * Check if the king can move to a position without threat
 ******************************************************************************/
int king_can_move(Piece *king)
{
	enum moveResult result = ILLEGAL;
	Piece *opponentPieces = get_opponent_pieces(king);

	int i;
	int j;

	for(i= -1 ; i<2; i++)
	{
		for(j= -1; j<2; j++)
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

/******************************************************************************
 * Check if the king can castle
 ******************************************************************************/
int can_castle(Piece *king, Piece *rook, int xd, int yd)
{
	int x_int = (king->x + xd) / 2;
	Piece *opponentPieces = get_opponent_pieces(king);

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

/******************************************************************************
 * Check if we can have a piece that protect the king
 ******************************************************************************/
void protect_king(Piece *king)
{
	int xLastPos = currentTurnInfo.last_move[0] - 'A';
	int yLasPos = currentTurnInfo.last_move[1] - 'A';
	Piece *threatPiece = boardGame[xLastPos][yLasPos];

	PieceType pieceType = get_last_pieceType_moved();

	switch (pieceType)
	{
		case KING:
				// TODO
			break;

		case ROOK:
			protect_king_from_vertical_attack(king , threatPiece);
			break;

		case BISHOP:
			protect_king_from_diagonal_attack(king , threatPiece);
			break;

		case KNIGHT:
			break;

		case QUEEN:
			protect_king_from_vertical_attack(king , threatPiece);
			protect_king_from_diagonal_attack(king , threatPiece);
			break;

		case PAWN:

			break;

		default:
			; //
		}
}

/******************************************************************************
 * Check if we can protect the king from diagonal attack
 ******************************************************************************/
static void protect_king_from_diagonal_attack(Piece *king , Piece *threatPiece)
{
	Piece *kingsPieces = get_own_pieces(king);

	int xIncrement = king->x > threatPiece->x ? 1 : -1 ;
	int yIncrement =  king->y > threatPiece->y ? 1 : -1 ;

	int i;
	int j;
	for(i = (threatPiece->x) ; (i =! king->x); (i += xIncrement) )
	{
		for(j = (threatPiece->y) ; (j =! king->y); (j += yIncrement) )
		{
			Piece *p = piece_can_move(kingsPieces,  i, j);
			if( p != 0)
			{
				place_the_piece_in_board(p->x, i, p->y, j, p);
				return;
			}
		}
	}

	return; // AUCUNE PIECE NE PEUX PROTEGER
}

/******************************************************************************
 * Check if we can protect the king from vertical attack
 ******************************************************************************/
static void protect_king_from_vertical_attack(Piece *king , Piece *threatPiece)
{
	Piece *kingsPieces = get_own_pieces(king);

	int increment = king->x > threatPiece->x ? 1 : -1 ;
	int i;

	for(i = (threatPiece->x) ; (i =! king->x); (i += increment))
	{
		Piece *p = piece_can_move(kingsPieces,  i, king->y);
		if( p != 0)
		{
			place_the_piece_in_board(p->x, i, p->y, king->y, p);
			return;
		}
	}

	return; // AUCUNE PIECE NE PEUX PROTEGER
}

/******************************************************************************
 * Check if a piece can move to a position
 ******************************************************************************/
static int piece_can_move_to(Piece *p, int xd, int yd)
{
	return (execute_move(p, p->x, xd, p->y, yd) != ILLEGAL ? 1 : 0);
}

/******************************************************************************
 * Return numbre of pieces that can move to a position
 ******************************************************************************/
static int pieces_can_move_to(Piece *playerPieces,  int xd, int yd)
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

/******************************************************************************
 * Return a piece that can move to a position
 ******************************************************************************/
static Piece* piece_can_move(Piece *playerPieces,  int xd, int yd)
{
	int i;
	Piece *p = 0;
	for(i = 0; i<16; i++)
	{
		p = &playerPieces[i];
		if(p->alive && piece_can_move_to(p,xd,yd))
		{
			return p;
		}
	}

	return p;
}

/******************************************************************************
 * Get own pieces of the specific piece
 ******************************************************************************/
static Piece *get_own_pieces(Piece *p)
{
	Piece *ownPieces;
	if(p->playerID == 1)
		ownPieces = player1Pieces;
	else
		ownPieces = player2Pieces;

	return ownPieces;
}

/******************************************************************************
 * Get opponent pieces of the specific piece
 ******************************************************************************/
static Piece *get_opponent_pieces(Piece *p)
{
	Piece *opponentPieces;
	if(p->playerID == 1)
		opponentPieces = player2Pieces;
	else
		opponentPieces = player1Pieces;

	return opponentPieces;
}



