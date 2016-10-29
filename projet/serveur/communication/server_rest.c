#include <stdio.h>
#include <string.h>

#include "json_from_struct.h"
#include "json_to_struct.h"
#include "server_http_out.h"

#define MAX_LENGTH 2048

enum request_type
{
	NEW_GAME,
	MOVE,
	PROMOTE,
	GET_TIME,
	GET_SUMMARY,
	GET_BOARD,
	POST_BOARD,
	GET_DETAILS,
	START,
	END,
	ERROR
};

int newGame_request(const char *data);
int move_request(const char *data);
int promote_request(const char *data);
int getTime_request(const char *data);
int getSummary_request();
int getBoard_request();
int postBoard_request(const char *data);
int getDetails_request();
int start_request();
int end_request(const char *data);
int read_token(const char *start, const char *end, char *dst);

/******************************************************************************
 * 
 *****************************************************************************/
int REST_handle_request(enum request_type type, const char *data)
{
	switch (type)
	{
	case NEW_GAME:
		return newGame_request(data);
	case MOVE:
		return move_request(data);
	case PROMOTE:
		return promote_request(data);
	case GET_TIME:
		return getTime_request(data);
	case GET_SUMMARY:
		return getSummary_request();
	case GET_BOARD:
		return getBoard_request();
	case POST_BOARD:
		return postBoard_request(data);
	case GET_DETAILS:
		return getDetails_request();
	case START:
		return start_request();
	case END:
		return end_request(data);
	case ERROR:
	default:
		return -1;
	}
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
int newGame_request(const char *data)
{
	/* read JSON and store into struct game_info */	
	struct game_info *info = parse_game_info(data);
	
	printf("- Player 1: %s\n", info->player_1);
	printf("- Player 2: %s\n", info->player_2);
	printf("- Round: %s\n", info->round);
	printf("- Location: %s\n", info->location);
	printf("- Secret code: %s\n", info->secret_code);
	printf("- Two tablets: %d\n", info->two_tablet);
	printf("- En passant: %d\n", info->en_passant);
	printf("- Timer format:\n");
	printf("\t- Time: %d\n", info->timer_format.time);
	printf("\t- Increment: %d\n", info->timer_format.increment);
	printf("\t- Limit: %d\n", info->timer_format.limit);
	printf("\t- Overtime: %d\n", info->timer_format.overtime);
	printf("\t- Overtime increment: %d\n\n", info->timer_format.overtime_increment);
	
	/* call something like: int new_game(struct game_info *info); */
	/* TODO */
	
	/* send answer to client via HTTP */
	HTTP_sendCode(/* valeur de retour precedente */ 200);
	
	return /* valeur de retour precedente */ 200;
}

/******************************************************************************
 * Asks chessboard to validate and move a piece.
 *
 * data contains the header of the request with format:
 *		POST /move/[1|2]/[a-h][1-8]-[a-h][1-8]
 *****************************************************************************/
int move_request(const char *data)
{	
	int player;
	char src[3], dst[3];
	static char response[MAX_LENGTH];
	
	/* split data into variables */
	sscanf(data, "POST /move/%d/%[^-]-%s\n", &player, src, dst);
	
	/* call something like: struct move_info *move_piece(int player, const char *src, const char *dst); */
	/* TODO */
	
	if (/* http status from returned struct == 200 */ 0 )
	{
		/* convert to JSON and send to client */
		move_info_to_json(/*info, response*/);
		HTTP_sendText(response);	
	}
	else
	{
		/* send error code to client via HTTP */
		HTTP_sendCode(/* valeur de retour precedente */ 200);
	}
	
	return /* valeur de retour precedente */ 200;
}

/******************************************************************************
 * Asks chessboard to promote a pawn.
 *
 * data contains the header of the request with format:
 *		POST /promote/[1|2]/[queen|bishop|rook|knight]
 *****************************************************************************/
int promote_request(const char *data)
{
	int player;
	char new_type[7];
	
	/* split data into variables */
	sscanf(data, "POST /promote/%d/%s\n", &player, new_type);
	
	/* call something like: int promote_piece(int player, const char *new_type); */
	/* TODO */
	
	/* send code to client via HTTP (200, 401 or 408) */
	HTTP_sendCode(/* valeur de retour precedente */ 200);
	
	return /* valeur de retour precedente */ 200;
}

/******************************************************************************
 * Asks remaining time for a player.
 *
 * data contains the header of the request with format:
 *		GET /time/[1|2]
 *****************************************************************************/
int getTime_request(const char *data)
{
	int player;
	static char response[MAX_LENGTH];
	
	/* get player number from data */
	sscanf(data, "GET /time/%d\n", &player);
	
	/* call something like: struct time_info *get_time(int player); */
	/* TODO */
	
	if (/* http status from returned struct == 200 */ 0 ) 
	{	
		/* convert to JSON and send to client */
		time_info_to_json(/*info, response*/);
		HTTP_sendText(response);	
	}
	else
	{
		/* send error code to client via HTTP */
		HTTP_sendCode(/* valeur de retour precedente */ 200);
	}
	
	return /* valeur de retour precedente */ 200;
}

/******************************************************************************
 * Asks the status of the game (summarized).
 *****************************************************************************/
int getSummary_request()
{
	static char response[MAX_LENGTH];
	
	/* call something like: struct turn_info *get_summary(); */
	/* TODO */
	
	if (/* http status from returned struct == 200 */ 0 ) 
	{
		/* convert to JSON and send to client */
		turn_info_to_json(/*info, response*/);
		HTTP_sendText(response);
	}
	else
	{
		/* send error code to client via HTTP */
		HTTP_sendCode(/* valeur de retour precedente */ 200);
	}
	
	return /* valeur de retour precedente */ 200;
}

/******************************************************************************
 * Asks the position of pieces on the chessboard.
 *****************************************************************************/
int getBoard_request()
{
	static char response[MAX_LENGTH];
	
	/* call something like: struct board_position *get_board(); */
	/* TODO */
	
	if (/* http status from returned struct == 200 */ 0 ) 
	{
		/* convert to JSON and send to client */
		board_position_to_json(/*info, response*/);
		HTTP_sendText(response);
	}
	else
	{
		/* send error code to client via HTTP */
		HTTP_sendCode(/* valeur de retour precedente */ 200);
	}
	
	return /* valeur de retour precedente */ 200;
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
int postBoard_request(const char *data)
{
	/* read JSON and store into struct board_position */
	struct board_position *board = parse_board_position(data);	
	
	int i;
	printf("- Turn: %d\n", board->turn);
	printf("- Move no: %d\n", board->move_no);
	for (i = 0; i < 32; i++)
		printf("%s\n", board->positions[i]);
	
	/* call something like: int set_board(struct board_position *board); */
	/* TODO */	
	
	/* send code to client via HTTP (200 or 401) */
	HTTP_sendCode(/* valeur de retour precedente */ 200);
	
	return /* valeur de retour precedente */ 200;
}

/******************************************************************************
 * Asks the details of the game (as set by player1).
 *****************************************************************************/
int getDetails_request()
{
	static char response[MAX_LENGTH];	
	
	/* call something like: struct game_info *get_game_info(); */
	/* TODO */
	
	if (/* http status from returned struct == 200 */ 0 ) 
	{
		/* convert to JSON and send to client */
		game_info_to_json(/*info, response*/);
		HTTP_sendText(response);
	}
	else
	{
		/* send error code to client via HTTP */
		HTTP_sendCode(/* valeur de retour precedente */ 200);
	}
	
	return /* valeur de retour precedente */ 200;
}

/******************************************************************************
 * Tell chessboard both player are ready to start the game.
 *****************************************************************************/
int start_request()
{
	/* call something like: int start_game(); */
	/* TODO */
	
	/* send code to client via HTTP (200 or 401) */
	HTTP_sendCode(/* valeur de retour precedente */ 200);
	
	return /* valeur de retour precedente */ 200;
}

/******************************************************************************
 * Tell chessboard to stop the game.
 *****************************************************************************/
int end_request(const char *data)
{
	/* call something like: int end_game(); */
	/* TODO */
	
	/* send code to client via HTTP (200 or 401) */
	HTTP_sendCode(/* valeur de retour precedente */ 200);
	
	return /* valeur de retour precedente */ 200;
}
