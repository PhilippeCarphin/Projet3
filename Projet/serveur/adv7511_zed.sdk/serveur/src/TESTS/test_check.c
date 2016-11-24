#include "test_check.h"
#include "chessboard.h"
#include "test_http.h"

void test_check()
{
	static int sequence = 1;
	MoveInfo mv;

	switch(sequence)
	{
		case 1:
			test_new_game(0);
			start_game();
			movePiece(1,"e2","e4",&mv);
			movePiece(2,"d7","d5",&mv);
			break;
		case 2:
			// Devrait mettre le roi noir en échec
			movePiece(1,"f1","b5",&mv);
			break;
		case 3:
			// Mettre un move qui devrait être refusé
			movePiece(2,"e7","e5",&mv);
			// Regarder mv pour voir,
			break;
		case 4:
			movePiece(2,"c7","c6",&mv);
			// Le move devrait etre accepté
			break;
		default:
			break;
	}

	sequence++;

}

void test_castling()
{
	static int sequence = 1;
	MoveInfo mv;

	switch(sequence)
	{
		case 1:
			test_new_game(0);
			start_game();
			movePiece(1,"e2","e4",&mv);
			movePiece(2,"d7","d5",&mv);
			break;
		case 2:
			// Devrait mettre le roi noir en échec
			movePiece(1,"f1","b5",&mv);
		case 3:
			// Mettre un move qui devrait être refusé
			movePiece(2,"e7","e5",&mv);
		case 4:
			movePiece(2,"c7","c6",&mv);
			// Le move devrait etre accepté
		case 5:
			movePiece(1,"e1","g1",&mv);
			break;
		case 6:
			movePiece(1,"g1","f3",&mv);
			break;
		case 7:
			movePiece(2,"g8","f6",&mv);
			break;
		case 8:
			movePiece(1,"e1","g1",&mv);
			break;
		default:
			break;
	}

	sequence++;

}

int test_castling2()
{
	static int sequence = 1;
	MoveInfo mv;

	switch(sequence)
	{
		case 1:
			test_new_game(0);
			start_game();
			movePiece(1,"e2","e4",&mv);
			movePiece(2,"d7","d5",&mv);
			movePiece(1,"f1","b5",&mv);
			movePiece(2,"c7","c6",&mv);
		case 2:
			// Esssayer le roque, devrait [etre refuse
			movePiece(1,"e1","g1",&mv);
			break;
		case 3:
			movePiece(1,"g1","f3",&mv);
			movePiece(2,"g8","f6",&mv);
			break;
		case 4:
			// Faire le roque, devrait etre accepte
			movePiece(1,"e1","g1",&mv);
			break;
		default:
			break;
	}

	sequence++;

}

