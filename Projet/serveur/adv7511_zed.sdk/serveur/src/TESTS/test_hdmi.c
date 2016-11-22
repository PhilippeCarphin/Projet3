#include "chessboard_rest_protocol.h"
#include "chessboard.h"
#include "chessclock.h"
#include "BoardDisplay.h"
#include "string.h"
#include "PieceEnum.h"

int new_game_test()
{
	static GameInfo gi;

	strcpy(gi.player_1,"Phil");
	strcpy(gi.player_2,"Sabrina");
	gi.round[0] = 0;
	gi.location[0] = 0;
	gi.secret_code[0] = 0;
	gi.en_passant = 1;

	gi.two_tablet = 0;

	gi.timer_format.time = 2600;
	gi.timer_format.increment = 22;
	gi.timer_format.limit = 40;
	gi.timer_format.overtime = 3600;
	gi.timer_format.overtime_increment = 11;

	new_game(&gi);

	return 0;
}


int test_hdmi()
{
	static int sequence = 1;

	MoveInfo mv;
	switch(sequence){
	case 1:
		new_game_test();
		break;
	case 2:
		start_game();
		break;
#if 0
	case 3:
		chessclock_1sec_callback();
		break;
	case 4:
		chessclock_1sec_callback();
		break;
	case 5:
		chessclock_1sec_callback();
		break;
	case 6:
		movePiece(WHITE,"e2","e4",&mv);
		break;
#endif
	default :
		chessclock_1sec_callback();
		break;

	}

	sequence++;
	return 0;
}


