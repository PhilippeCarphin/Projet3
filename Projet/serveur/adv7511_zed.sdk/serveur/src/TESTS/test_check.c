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
