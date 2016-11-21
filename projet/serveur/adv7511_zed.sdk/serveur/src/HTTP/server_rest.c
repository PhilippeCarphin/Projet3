#include "server_rest.h"
#include "json_from_struct.h"
#include "json_to_struct.h"
#include "macros.h"
#include "chessboard.h"
#include "base64_converter.h"
#include "debug.h"

//PHIL TEMP
#include "BoardDisplay.h"

#include <stdio.h>

static const int MAX_lEN = 128;

int newGame_request(const char *data, char *REST_response);
int move_request(const char *data, char *REST_response);
int promote_request(const char *data, char *REST_response);
int getTime_request(const char *data, char *REST_response);
int getSummary_request(char *REST_response);
int getBoard_request(char *REST_response);
int postBoard_request(const char *data, char *REST_response);
int getDetails_request(char *REST_response);
int start_request(char *REST_response);
int join_request(char *REST_response);
int end_request(const char *data, char *REST_response);

/******************************************************************************
 * Uses a big switch-case block to send data to the right handling function.
 *****************************************************************************/
int REST_handle_request(enum request_type type, const char *data,
							  const char *pswd, char *REST_response)
{
	FBEGIN
	char pswd_b64[MAX_lEN];
	pswd_b64[0] = '\0';
	ascii_to_base64(pswd, strlen(pswd), pswd_b64);

	/*
	 * Validate password for certain types of requests
	 */
	switch(type)
	{
		case MOVE:
		case PROMOTE:
		case POST_BOARD:
		case START:
		case END:
		case JOIN:
			if (validate_password(pswd_b64) == unathorized)
			{
				return unathorized;
			}
			break;
		default:
			break;
	}
	
	/*
	 * Switch to correct request handler based on request type
	 */
	switch (type)
	{
	case NEW_GAME:
		return newGame_request(data, REST_response);
	case MOVE:
		return move_request(data, REST_response);
	case PROMOTE:
		return promote_request(data, REST_response);
	case GET_TIME:
		return getTime_request(data, REST_response);
	case GET_SUMMARY:
		return getSummary_request(REST_response);
	case GET_BOARD:
		return getBoard_request(REST_response);
	case POST_BOARD:
		return postBoard_request(data, REST_response);
	case GET_DETAILS:
		return getDetails_request(REST_response);
	case START:
		return start_request(REST_response);
	case END:
		return end_request(data, REST_response);
	case JOIN:
		return join_request(REST_response);
	case ERROR:
	default:
		return -1;
	}
	FEND
}

/******************************************************************************
 * Asks chessboard to create a new game with the configuration specified in data 
 *
 * data contains a JSON file with format:
 *	 {
 *		"player1": string,
 *		"player2": string,
 *		"round": string,
 *		"location": string,
 *		"secret_code": string,
 *		"twoTablet": [yes|no],
 *		"enPassant": [yes|no],
 *		"timerFormat": {
 *			"time":		integer,
 *			"increment":integer,
 *			"limit":	integer,
 *			"overtime":	integer,
 *			"overtimeIncrement": integer
 *		}
 *	 }
 *****************************************************************************/
int newGame_request(const char *data, char *REST_response)
{
	/* read JSON and store into struct game_info */	
	struct GameInfo info;
	parse_game_info(data, &info);

	/* convert password into base64 */
	char pswd_b64[MAX_lEN];
	ascii_to_base64(info.secret_code, strlen(info.secret_code), pswd_b64);
	strcpy(info.secret_code, pswd_b64);

	/*
	new_game(struct game_info *gi)
	{
		if partie en cours
			return PARTIE_EN_COURS;
		else
			memcpy(&game_info, gi, sizeof(struct game_info));
			return OK

	}

	*/
	
	PRINT("- Player 1: %s\n", info.player_1);
	PRINT("- Player 2: %s\n", info.player_2);
	PRINT("- Round: %s\n", info.round);
	PRINT("- Location: %s\n", info.location);
	PRINT("- Secret code: %s\n", info.secret_code);
	PRINT("- Two tablets: %d\n", info.two_tablet);
	PRINT("- En passant: %d\n", info.en_passant);
	PRINT("- Timer format:\n");
	PRINT("\t- Time: %d\n", info.timer_format.time);
	PRINT("\t- Increment: %d\n", info.timer_format.increment);
	PRINT("\t- Limit: %d\n", info.timer_format.limit);
	PRINT("\t- Overtime: %d\n", info.timer_format.overtime);
	PRINT("\t- Overtime increment: %d\n\n", info.timer_format.overtime_increment);
	
	/* call something like: int new_game(struct game_info *info); */
	int status = new_game(&info);
	
	REST_response[0] = '\0';

	/* send answer to client via HTTP */
	return status;
}

/******************************************************************************
 * Asks chessboard to validate and move a piece.
 *
 * data contains the header of the request with format:
 *		POST /move/[1|2]/[a-h][1-8]-[a-h][1-8]
 *****************************************************************************/
int move_request(const char *data, char *REST_response)
{	
	int player;
	char src[3], dst[3];
	
	/* split data into variables */
	sscanf(data, "POST /move/%d/%[^-]-%s\n", &player, src, dst);
	
	/* call something like: struct move_info *move_piece(int player, const char *src, const char *dst); */
	MoveInfo info;
	int status = movePiece(player, src, dst, &info);
	
	if ( status == OK )
	{
		/* convert to JSON and send to client */
		move_info_to_json(info, REST_response);
	}

	/* send status code to client */
	return status;
}

/******************************************************************************
 * Asks chessboard to promote a pawn.
 *
 * data contains the header of the request with format:
 *		POST /promote/[1|2]/[queen|bishop|rook|knight]
 *****************************************************************************/
int promote_request(const char *data, char *REST_response)
{
	int player;
	char new_type[7];
	
	/* split data into variables */
	sscanf(data, "POST /promote/%d/%s\n", &player, new_type);
	
	/* call something like: int promote_piece(int player, const char *new_type); */
	int status = promote_piece(player, new_type);
	
	/* send code to client via HTTP (200, 401 or 408) */
	REST_response[0] = '\0';
	return status;
}

/******************************************************************************
 * Asks remaining time for a player.
 *
 * data contains the header of the request with format:
 *		GET /time/[1|2]
 *****************************************************************************/
int getTime_request(const char *data, char *REST_response)
{
	int player;
	
	/* get player number from data */
	sscanf(data, "GET /time/%d\n", &player);
	
	/* call something like: struct time_info *get_time(int player); */
	struct TimeInfo info;
	int status = get_time(player, &info);
	
	if (status == OK)
	{	
		/* convert to JSON and send to client */
		time_info_to_json(info, REST_response);
	}
	/* send status code to client */
	return status;
}

/******************************************************************************
 * Asks the status of the game (summarized).
 *****************************************************************************/
int getSummary_request(char *REST_response)
{
	/* call something like: struct turn_info *get_summary(); */
	struct TurnInfo info;
	int status = get_summary(&info);
	
	if (status == OK)
	{
		/* convert to JSON and send to client */
		turn_info_to_json(info, REST_response);
	}
	/* send status code to client */
	return status;
}

/******************************************************************************
 * Asks the position of pieces on the chessboard.
 *****************************************************************************/
int getBoard_request(char *REST_response)
{
	/* call something like: struct board_position *get_board(); */
	struct BoardPosition board;
	int status = get_board(&board);
	
	if (status == OK)
	{
		/* convert to JSON and send to client */
		board_position_to_json(board, REST_response);
	}
	/* send status code to client */

#if 0

	/*
	 * The HDMI is updated in BoardDisplay_move_piece().  This causes the screen
	 * buffer, which is modified in BoardDisplay_move_piece(), to be sent to
	 * the screen by the cf_hdmi module.
	 *
	 * The ChessBoard_move_piece() function, after validating that the move is
	 * legal, will declare a struct Move mv, populate it's fields with the
	 * information of the move needed by BoardDisplay, and call BoardDisplay_move_piece()
	 * and with &mv to tell it where the move information is stored.
	 */
	test_move_piece();

#endif

	return status;
}

/******************************************************************************
 * Gives chessboard the position of pieces.
 *
 * data contains a JSON file with format:
 *	 {
 *		"turn": [1|2],
 *		"move_no": integer,
 *		"king1":	[a-h][1-8],
 *		"queen1":	[a-h][1-8] | X,
 *		"bishop1A":	[a-h][1-8] | X,
 *		"bishop1B":	[a-h][1-8] | X,
 *		"rook1A":	[a-h][1-8] | X,
 *		"rook1B":	[a-h][1-8] | X,
 *		"knight1A":	[a-h][1-8] | X,
 *		"knight1B":	[a-h][1-8] | X,
 *		"pawn1":	[ [a-h][1-8] | X, ..., [a-h][1-8] | X ],
 *		
 *		"king2":	[a-h][1-8],
 *		...
 *	 }
 *****************************************************************************/
int postBoard_request(const char *data, char *REST_response)
{
	/* read JSON and store into struct board_position */
	struct BoardPosition board;
	parse_board_position(data, &board);
	
	int i;
	PRINT("- Turn: %d\n", board.turn);
	PRINT("- Move no: %d\n", board.move_no);
	for (i = 0; i < 32; i++)
		PRINT("%c%c\n", board.positions[i][0], board.positions[i][1]);
	
	/* call something like: int set_board(struct board_position *board); */
	int status = set_board(&board);
	
	/* send code to client via HTTP (200 or 401) */
	REST_response[0] = '\0';
	return status;
}

/******************************************************************************
 * Asks the details of the game (as set by player1).
 *****************************************************************************/
int getDetails_request(char *REST_response)
{
	/* call something like: struct game_info *get_game_info(); */
	struct GameInfo info;
	int status = get_game_info(&info);
	
	if (status == OK)
	{
		/* convert to JSON and send to client */
		game_info_to_json(info, REST_response);
	}
	/* send status code to client */
	return status;
}

/******************************************************************************
 * Tell chessboard both player are ready to start the game.
 *****************************************************************************/
int start_request(char *REST_response)
{
	/* call something like: int start_game(); */
	int status = start_game();
	
	/* send code to client via HTTP (200 or 401) */
	REST_response[0] = '\0';
	return status;
}

/******************************************************************************
 *
 *****************************************************************************/
int join_request(char *REST_response)
{
	/* call something like: int start_game(); */
	int status = join_game();

	/* send code to client via HTTP (200 or 401) */
	REST_response[0] = '\0';
	return status;
}

/******************************************************************************
 * Tell chessboard to stop the game.
 *****************************************************************************/
int end_request(const char *data, char *REST_response)
{
	/* call something like: int end_game(); */
	int status = end_game();
	
	/* send code to client via HTTP (200 or 401) */
	REST_response[0] = '\0';
	return status;
}
