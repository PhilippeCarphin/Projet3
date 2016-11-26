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
static Piece *get_own_pieces(Piece *p);
static Piece *get_opponent_pieces(Piece *p);
static Piece *get_last_piece_moved();
static int piece_can_move_to(Piece *p, int xd, int yd);
static int pieces_can_move_to(Piece *playerPieces,  int xd, int yd);
static int pieces_can_move_to_interpose(Piece *player_pieces, int xd, int yd);

static int can_interpose_vertical_attack(Piece *king, Piece *threat_piece);
static int can_interpose_latteral_attack(Piece *king, Piece *threat_piece);
static int can_interpose_diagonal_attack(Piece *king, Piece *threat_piece);

int a_piece_can_interpose(Piece *king, Piece *threat_piece);

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
 * This function should only be called if the king is in check.  Therefore
 * we assume that there is at least one piece attacking the king.
 ******************************************************************************/
int check_mate(Piece *king)
{
	if(king_can_move(king))
		return 0;

	/*
	 * king cannot move, we must try to solve the check by interposition
	 */
	Piece *enemyPieces = get_opponent_pieces(king);
	int nb = pieces_can_move_to(enemyPieces,king->x,king->y);

	/*
	 * If the king cannot move and there are two pieces attacking it, then
	 * we cannot block the check by interposition ==> checkmate.
	 */
	if(nb == 2)
		return 1;

	/*
	 * Now there is only one piece attacking the king.  Logic dictates that this
	 * is the last piece that was moved.
	 */
	Piece *threat_piece = get_last_piece_moved();
	if(a_piece_can_interpose(king,threat_piece))
	{
		return 0;
	}
	else
	{
		return 1;
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
				// we have to try the move
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

static Piece *get_last_piece_moved()
{
	int xLastPos = currentTurnInfo.last_move_dst[0] - 'A';
	int yLasPos = currentTurnInfo.last_move_dst[1] - 'A';
	return boardGame[xLastPos][yLasPos];
}





int a_piece_can_interpose(Piece *king, Piece *threat_piece)
{
	Piece *kingsPieces = get_own_pieces(king);

	if(threat_piece->pieceType == KNIGHT || threat_piece->pieceType == PAWN)
	{
		if(pieces_can_move_to_interpose(kingsPieces, threat_piece->x, threat_piece->y))
			return 1;
	}
	else
	{
		int dx = king->x - threat_piece->x;
		int dy = king->y - threat_piece->y;

		if(dx == 0 && dy != 0)
		{
			if(can_interpose_vertical_attack(king, threat_piece))
				return 1;
		}
		else if (dx != 0 && dy == 0)
		{
			if(can_interpose_latteral_attack(king, threat_piece))
				return 1;
		}
		else if (dx != 0 && dy != 0)
		{
			if(can_interpose_diagonal_attack(king, threat_piece))
				return 1;
		}
	}

	return 0;

}

/******************************************************************************
 * Check if the king can be protected from a vertical attack by interposition
 * of one of his pieces between the threat and the king.
 ******************************************************************************/
static int can_interpose_vertical_attack(Piece *king, Piece *threat_piece)
{
	Piece *kingsPieces = get_own_pieces(king);
	int incr = (king->y > threat_piece->y ? 1 : -1);
	int y;
	for(y = threat_piece->y; y != king->y; y += incr)
	{
		if(pieces_can_move_to_interpose(kingsPieces,king->x,y))
		{
			/*
			 * we found a piece that can protect the king from the threat
			 * That's all we need to know, we don't make the move for the
			 * player.
			 */
			return 1;
		}
	}
	/*
	 * No piece can move to any of the squares between the king and
	 * the threat. Therefore, the king cannot be protected by interposition.
	 */
	return 0;
}

/******************************************************************************
 * Check if the king can be protected from a latteral attack by interposition
 * of one of his pieces between the threat and the king.
 ******************************************************************************/
static int can_interpose_latteral_attack(Piece *king, Piece *threat_piece)
{
	Piece *kingsPieces = get_own_pieces(king);
	int xIncrement = (king->x > threat_piece->x ? 1 : -1);
	int x;
	for(x = threat_piece->x; x != king->x; x += xIncrement)
	{
		if(pieces_can_move_to_interpose(kingsPieces,x,king->y))
		{
			/*
			 * we found a piece that can protect the king from the threat
			 * That's all we need to know, we don't make the move for the
			 * player.
			 */
			return 1;
		}
	}
	/*
	 * No piece can move to any of the squares between the king and
	 * the threat. Therefore, the king cannot be protected by interposition.
	 */
	return 0;
}

/******************************************************************************
 * Check if the king can be protected from a diagonal attack by interposition
 * of one of his pieces between the threat and the king.
 ******************************************************************************/
static int can_interpose_diagonal_attack(Piece *king , Piece *threatPiece)
{
	Piece *kingsPieces = get_own_pieces(king);

	int xIncrement = king->x > threatPiece->x ? 1 : -1 ;
	int yIncrement = king->y > threatPiece->y ? 1 : -1 ;

	int x;
	int y;
	for(x = (threatPiece->x) ; (x =! king->x); (x += xIncrement) )
	{
		for(y = (threatPiece->y) ; (y =! king->y); (y += yIncrement) )
		{
			if(pieces_can_move_to_interpose(kingsPieces,x,king->y))
			{
				/*
				 * we found a piece that can protect the king from the threat
				 * That's all we need to know, we don't make the move for the
				 * player.
				 */
				return 1;
			}
		}
	}
	/*
	 * No piece can move to any of the squares between the king and
	 * the threat. Therefore, the king cannot be protected by interposition.
	 */
	return 0;
}

/******************************************************************************
 ********************* Sub-functions to try moves *****************************
 ******************************************************************************/
void try_move(Piece *piece, Piece *old_piece, int xs, int ys, int xd, int yd)
{
	old_piece= boardGame[xd][yd];
	if(old_piece != 0)
	{
		// If there was a piece at xd,yd, then that piece is alive
		old_piece->alive = 0;
	}
	boardGame[xd][yd] = piece;
	boardGame[xs][ys] = 0;
	piece->x = xd;
	piece->y = xd;
}

void undo_try_move(Piece *piece, Piece *old_piece, int xs, int ys, int xd, int yd)
{
	boardGame[xd][yd] = old_piece;
	boardGame[xs][ys] = piece;
	piece->x = xs;
	piece->y = xs;
	if(old_piece != 0)
	{
		// If there was a piece at xd,yd, then that piece is alive
		old_piece->alive = 1;
	}
}
/******************************************************************************
 * One thing that can stop a piece from moving is if the piece is pinned
 * https://en.wikipedia.org/wiki/Pin_(chess).  If a piece is pinned, just
 * anywhere (it is not completely fixed; it can move as long as the king
 * is not in check after it moved.  Therefore we must specify the move we want
 * to make and this function will tell us if the king is in check after the move.
 ******************************************************************************/
static int in_check_after_move(Piece *piece, int xd, int yd)
{
	int in_check;
	/*
	 * The move seems to be legal, but we need to verify if the king is in
	 * check after the move.  Because if that is the case, the the piece can't
	 * move so we'll just do like we did in movePiece(), which is to make the
	 * move, verify if the king is in check, and undo it.
	 */

	/*
	 * Record the origin square of the piece to put it back after
	 */
	int xs = piece->x;
	int ys = piece->y;

	/*
	 * Move the piece
	 */
	Piece *old_piece= boardGame[xd][yd];
	try_move(piece, old_piece, xs, ys, xd, yd);

	/*
	 * If after the move we are in check, then we conclude that the piece
	 * cannot move.
	 */
	in_check = king_is_in_check(get_own_pieces(piece));

	/*
	 * We got our answer: put the piece back.  This module is only there to
	 * answer questions, it should not modify the board except temporarily
	 */
	undo_try_move(piece, old_piece, xs, ys, xd, yd);

	return in_check;
}

/******************************************************************************
 **************** MOVE CHECK FUNCTIONS ****************************************
 ******************************************************************************/
/*
 * The first one is used to verify if the king is in check.  It verifies if a
 * piece can move to the specified square without verifying if it's king would
 * be in check after the move. Indeed, that is how verifying if a king is in
 * check works.  Even if the piece can't move, as long as it attacks the enemy
 * king, the enemy kning will be in check
 *
 * The next function is used for determining if the king checkmate state.  To
 * determine if the king is in checkmate, we will first determine if the king
 * can move to get out of check.  If that is the case, the we're done.  However
 * if the king cannot move, then we must check if a piece can move to interpose:
 *
 * For a piece to be able to interpose, it's king must not be in check after it
 * moves to interpose.  That is why there are two functions.
 */

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
 * Returns 1 if there is a piece that can move to interpose
 ******************************************************************************/
static int pieces_can_move_to_interpose(Piece *player_pieces, int xd, int yd)
{
	int i;
	for(i = 1; i < 16; ++i)
	{
		Piece *p = &player_pieces[i];
		if(piece_can_move_to(p,xd,yd) && !in_check_after_move(p,xd,yd))
			return 1;
	}
	return 0;
}

static int piece_can_move_to(Piece *p, int xd, int yd)
{
	return (execute_move(p, p->x, xd, p->y, yd) != ILLEGAL ? 1 : 0);
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



