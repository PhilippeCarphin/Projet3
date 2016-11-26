#include "test_check.h"
#include "chessboard.h"
#include "test_http.h"
#define DEBUG
#include "debug.h"

void test_check()
{
	static int sequence = 1;
	MoveInfo mv;

	switch(sequence)
	{
		case 1:
			new_game_test();
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
			new_game_test();
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
			new_game_test();
			start_game();
			movePiece(1,"e2","e4",&mv);
			movePiece(2,"d7","d5",&mv);
			movePiece(1,"f1","b5",&mv);
			movePiece(2,"c7","c6",&mv);
			break;
		case 2:
			// Esssayer le roque, devrait [etre refuse
			WHERE DBG_PRINT("Case 2: trying caslting\n");
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
	return 0;

}

int test_castling3()
{
	static int sequence = 1;
	MoveInfo mv;

	switch(sequence)
	{
		case 1:
			/*
			 * Setup test position
			 */
			new_game_test();
			start_game();
			movePiece(1,"e2","e4",&mv);
			movePiece(2,"d7","d5",&mv);
			movePiece(1,"f1","b5",&mv);
			movePiece(2,"c7","c6",&mv);

			movePiece(1,"g1","f3",&mv);
			movePiece(2,"c8","h3",&mv);
			movePiece(1,"g2","g3",&mv);
			movePiece(2,"c6","b5",&mv);
			break;
		case 2:
			/*
			 * Essayer le roque.  C'est illegal "Castling through check"
			 */
			WHERE DBG_PRINT("Case 2: trying caslting\n");
			movePiece(1,"e1","g1",&mv);
			break;
		case 3:
			/*
			 * Faire que ça ne soit pas illegal
			 */
			movePiece(1,"f3","g5",&mv);
			movePiece(2,"h3","g4",&mv);
			break;
		case 4:
			/*
			 * Essayer le roque. Devrait être accepté.
			 */
			movePiece(1,"e1","g1",&mv);
			break;
		default:
			break;
	}

	sequence++;
	return 0;

}

int test_castling4()
{
	static int sequence = 1;
	MoveInfo mv;

	switch(sequence)
	{
		case 1:
			/*
			 * Setup test position
			 */
			new_game_test();
			start_game();

			movePiece(1,"e2","e4",&mv);
			movePiece(2,"d7","d5",&mv);
			movePiece(1,"e4","d5",&mv);
			movePiece(2,"d8","d5",&mv);

			movePiece(1,"f1","d3",&mv);
			movePiece(2,"h7","h6",&mv);
			movePiece(1,"g1","h3",&mv);
			movePiece(2,"d5","e5",&mv);
			break;
		case 2:
			/*
			 * Essayer le roque.  C'est illegal "Castling through check"
			 */
			WHERE DBG_PRINT("Case 2: trying caslting\n");
			movePiece(1,"e1","g1",&mv);
			break;
		case 3:
			/*
			 * Faire que ça ne soit pas illegal
			 */
			movePiece(1,"d3","e2",&mv);
			movePiece(2,"h6","h5",&mv);
			break;
		case 4:
			/*
			 * Essayer le roque. Devrait être accepté.
			 */
			movePiece(1,"e1","g1",&mv);
			break;
		default:
			break;
	}

	sequence++;
	return 0;

}
int test_queenside_castling()
{
	static int sequence = 1;
	MoveInfo mv;

	switch(sequence)
	{
		case 1:
			/*
			 * Setup test position
			 */
			new_game_test();
			start_game();
			// 1. d4 e5
			movePiece(1,"d2","d4",&mv);
			movePiece(2,"e7","e5",&mv);

			// 2. Qd3 Qf6
			movePiece(1,"d1","d3",&mv);
			movePiece(2,"d8","f6",&mv);

			// 3. Bd2 Qd6
			movePiece(1,"c1","d2",&mv);
			movePiece(2,"f6","d6",&mv);

			// 4. Nc3 h6
			movePiece(1,"b1","c3",&mv);
			movePiece(2,"h7","h6",&mv);

			// 5. b3 Qa3
			movePiece(1,"b2","b3",&mv);
			movePiece(2,"d6","a3",&mv);
			break;
		case 2:
			/*
			 * Essayer le grand roque"
			 */
			WHERE DBG_PRINT("Case 2: trying caslting\n");
			movePiece(1,"e1","c1",&mv);
			break;
		case 3:
			/*
			 * Faire que ça ne soit pas illegal
			 */
			movePiece(1,"c3","b5",&mv);
			movePiece(2,"a3","b4",&mv);
			break;
		case 4:
			/*
			 * Essayer le roque. Devrait être accepté.
			 */
			movePiece(1,"e1","c1",&mv);
			break;
		default:
			break;
	}

	sequence++;
	return 0;

}
