#include <string.h>
#include <stdio.h>

#include "chessboard_rest_protocol.h"

/******************************************************************************
 *
 *****************************************************************************/
int set_yes_no(int boolean, char *buf)
{
	if (boolean)
	{
		return sprintf(buf, "%s", "yes");
	}
	else
	{
		return sprintf(buf, "%s", "no");
	}
}

/******************************************************************************
 *
 *****************************************************************************/
int set_state(enum State state, char *buf)
{
	switch (state)
	{
	case NORMAL:
		return sprintf(buf, "%s", "normal");
	case CHECKMATE:
		return sprintf(buf, "%s", "checkmate");
	case CHECK:
		return sprintf(buf, "%s", "check");
	case STALEMATE:
		return sprintf(buf, "%s", "stalemate");
	default:
		return -1;
	}
}

/******************************************************************************
 *
 *****************************************************************************/
int set_pieces(char positions[][2], char pieces[][2][20])
{
	int i = 0;
	static const char position_names[][20] = {
		"king1", "queen1", "bishop1A", "bishop1B", "rook1A", "rook1B", "knight1A", "knight1B",
		"king2", "queen2", "bishop2A", "bishop2B", "rook2A", "rook2B", "knight2A", "knight2B"
	};

	for (i = 0; i < 8; i++)
	{
		sprintf(pieces[i][0], "\"%s\": %c%c", position_names[i], positions[i][0], positions[i][1]);
	}
	sprintf(pieces[8][0], "\"pawn1\": [ %c%c, %c%c, %c%c, %c%c, %c%c, %c%c, %c%c, %c%c ]",
			positions[8][0],  positions[8][1],  positions[9][0],  positions[9][1],
			positions[10][0], positions[10][1],	positions[11][0], positions[11][1],
			positions[12][0], positions[12][1],	positions[13][0], positions[13][1],
			positions[14][0], positions[14][1],	positions[15][0], positions[15][1]
	);

	for (i = 0; i < 8; i++)
	{
		sprintf(pieces[i][1], "\"%s\": %c%c", position_names[i+8], positions[i+8][0], positions[i+8][1]);
	}
	sprintf(pieces[8][1], "\"pawn2\": [ %c%c, %c%c, %c%c, %c%c, %c%c, %c%c, %c%c, %c%c ]",
			positions[8][0],  positions[8][1],  positions[9][0],  positions[9][1],
			positions[10][0], positions[10][1],	positions[11][0], positions[11][1],
			positions[12][0], positions[12][1],	positions[13][0], positions[13][1],
			positions[14][0], positions[14][1],	positions[15][0], positions[15][1]
	);

	return 0;
}

/******************************************************************************
 * 
 *****************************************************************************/
int move_info_to_json(MoveInfo info, char *json)
 {
	char promotion[4];
	char state[10];
	int err[2];

	err[0] = set_yes_no(info.promotion, promotion);
	err[1] = set_state(info.game_status, state);

	if (err[0] < 0 || err[1] < 0)
		return -1;

	/* fill json string from values in struct */
	return sprintf(json,
			"{\"pieceEliminated\": %c%c, \"promotion\": %s, \"state\": %s}",
			info.piece_eliminated[0], info.piece_eliminated[1], promotion, state);
 }
 
/******************************************************************************
 *
 *****************************************************************************/
 int time_info_to_json(TimeInfo info, char *json)
 {
	/* fill json string from values in struct */
	return sprintf(json, "{\"time\": %d, \"overtime\": %d}",
			info.time, info.overtime);
 }
 
/******************************************************************************
 *
 *****************************************************************************/
 int turn_info_to_json(TurnInfo info, char *json)
 {
	 char state[10];
	 int err;

	 err = set_state(info.game_status, state);

	 if (err < 0)
		 return -1;

	/* fill json string from values in struct */
	return sprintf(json,
			"{\"turn\": %d, \"moveNo\": %d, \"lastMove\": %s, \"state\": %s}",
			info.turn, info.move_no, info.last_move, state);
 }
 
/******************************************************************************
 *
 *****************************************************************************/
 int board_position_to_json(BoardPosition board, char *json)
 {
	/* fill json string from values in struct */
	char pieces[8][2][20];	/* does not contain pawns */

	set_pieces(board.positions, pieces);

	return sprintf(json,
			"{\"turn\": %d, \"move_no\": %d, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s}",
			board.turn, board.move_no,
			pieces[0][0], pieces[1][0], pieces[2][0], pieces[3][0],
			pieces[4][0], pieces[5][0], pieces[6][0], pieces[7][0],
			pieces[8][0], pieces[0][1], pieces[1][1], pieces[2][1],
			pieces[3][1], pieces[4][1], pieces[5][1], pieces[6][1],
			pieces[7][1], pieces[8][1]
	);
 }
 
/******************************************************************************
 *
 *****************************************************************************/
 int game_info_to_json(GameInfo info, char *json)
 {
	char en_passant[4];

	int err = set_yes_no(info.en_passant, en_passant);

	if (err < 0)
		return -1;

	/* fill json string from values in struct */
	return sprintf(json,
			"{\"player1\": %s, \"player2\": %s, \"round\": %s, \"location\": %s, \"enPassant\": %s, \"timerFormat\": {\"time\": %d, \"increment\": %d, \"limit\": %d, \"overtime\": %d, \"overtimeIncrement\": %d}}",
			info.player_1, info.player_2, info.round, info.location, en_passant,
			info.timer_format.time, info.timer_format.increment, info.timer_format.limit,
			info.timer_format.overtime, info.timer_format.overtime_increment
	);

 }
