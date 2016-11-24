#include "json_from_struct.h"
#include "macros.h"

#include <string.h>
#include <stdio.h>

static const int MAX_STR_LEN = 50;

/******************************************************************************
 * Transforms a C boolean (0 or 1) into its JSON equivalent (yes or no).
 *****************************************************************************/
int set_yes_no(int boolean, char *buf)
{
	if (boolean == 1)
	{
		return sprintf(buf, "%s", "yes");
	}
	else
	{
		return sprintf(buf, "%s", "no");
	}
}

/******************************************************************************
 * Puts the enum State into words, for JSON.
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
 * From given positions, creates the individual JSON fields into the pieces array.
 * JSON fields are of format:
 * 	"king1": a1
 * for non-pawn pieces, while pawns are displayed in format:
 * 	"pawn1": [ b1, b2, b4, b5, c8, d6, e3, f7 ]
 *****************************************************************************/
int set_pieces(char positions[32][2], char fields[18][MAX_STR_LEN])
{
	int i = 0;
	static const char position_names[][20] =
	{
		"king1", "queen1", "bishop1A", "bishop1B", "rook1A", "rook1B", "knight1A", "knight1B",
		"king2", "queen2", "bishop2A", "bishop2B", "rook2A", "rook2B", "knight2A", "knight2B"
	};

	/* king1 to knight1B */
	for (i = 0; i < 8; i++)
	{
		sprintf(fields[i], "\"%s\": %c%c", position_names[i], positions[i][0], positions[i][1]);
	}
	/* pawn1 */
	sprintf(fields[8], "\"pawn1\": [ %c%c, %c%c, %c%c, %c%c, %c%c, %c%c, %c%c, %c%c ]",
			positions[8][0],  positions[8][1],  positions[9][0],  positions[9][1],
			positions[10][0], positions[10][1],	positions[11][0], positions[11][1],
			positions[12][0], positions[12][1],	positions[13][0], positions[13][1],
			positions[14][0], positions[14][1],	positions[15][0], positions[15][1]
	);
	/* king2 to knight2B */
	for (i = 9; i < 17; i++)
	{
		sprintf(fields[i], "\"%s\": %c%c", position_names[i-1], positions[i+7][0], positions[i+7][1]);
	}
	/* pawn2 */
	sprintf(fields[17], "\"pawn2\": [ %c%c, %c%c, %c%c, %c%c, %c%c, %c%c, %c%c, %c%c ]",
			positions[24][0], positions[24][1], positions[25][0], positions[25][1],
			positions[26][0], positions[26][1],	positions[27][0], positions[27][1],
			positions[28][0], positions[28][1],	positions[29][0], positions[29][1],
			positions[30][0], positions[30][1],	positions[31][0], positions[31][1]
	);

	return 0;
}

/******************************************************************************
 * Transforms a MoveInfo struct into its JSON equivalent.
 *****************************************************************************/
int move_info_to_json(MoveInfo info, char *json)
 {
	char promotion[4];	/* [yes|no] */
	char state[10];		/* [normal|checkmate|check|stalemate] */
	int err[2];			/* return values of following functions */

	/* change integer and enum values into strings */
	err[0] = set_yes_no(info.promotion, promotion);
	err[1] = set_state(info.game_status, state);
	if (err[0] < 0 || err[1] < 0)
	{
		return -1;
	}

	/* fill json string from values in struct */
	return sprintf(json,
			"{\"pieceEliminated\": %c%c, \"promotion\": %s, \"state\": %s}",
			info.piece_eliminated[0], info.piece_eliminated[1], promotion, state);
 }
 
/******************************************************************************
 * Transforms a TimeInfo struct into its JSON equivalent.
 *****************************************************************************/
 int time_info_to_json(TimeInfo info, char *json)
 {
	/* fill json string from values in struct */
	return sprintf(json, "{\"time\": %d, \"overtime\": %d}",
			info.time, info.overtime);
 }
 
/******************************************************************************
 * Transforms a TurnInfo struct into its JSON equivalent.
 *****************************************************************************/
 int turn_info_to_json(TurnInfo info, char *json)
 {
	 char state[10];	/* [normal|checkmate|check|stalemate] */
	 int err;			/* return value of set_state */

	/* change enum value into string */
	 err = set_state(info.game_status, state);
	 if (err < 0)
	 {
		 return -1;
	 }

	/* fill json string from values in struct */
	return sprintf(json,
			"{\"turn\": %d, \"moveNo\": %d, \"lastMoveSrc\": %c%c, \"lastMoveDst\": %c%c, \"state\": %s}",
			info.turn, info.move_no, info.last_move_src[0], info.last_move_src[1],
			info.last_move_dst[0], info.last_move_dst[1], state);
 }
 
/******************************************************************************
 * Transforms a BoardPosition struct into its JSON equivalent.
 *****************************************************************************/
 int board_position_to_json(BoardPosition board, char *json)
 {
	 char state[10];	/* [normal|checkmate|check|stalemate] */
	 int err;			/* return value of set_state */

	 /* change enum value into string */
	 err = set_state(board.turn_info.game_status, state);
	 if (err < 0)
	 {
		 return -1;
	 }

	 /*
	  * All pieces but pawns, for both players:
	  * 	16 pieces, requiring 16 fields.
	  * Remaining pawns are split into 2 groups (player 1 & 2):
	  * 	8 pieces per group, 2 groups, requiring 2 fields.
	  */
	char fields[18][MAX_STR_LEN];

	/* format individual pieces into JSON, so its less of a huge one-liner */
	set_pieces(board.positions, fields);

	/* fill json string from values in struct */
	return sprintf(json,
			"{\"turn\": %d, \"move_no\": %d, \"lastMoveSrc\": %c%c, \"lastMoveDst\": %c%c, \"state\": %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s}",
			board.turn_info.turn, board.turn_info.move_no, board.turn_info.last_move_src[0], board.turn_info.last_move_src[1],
			board.turn_info.last_move_dst[0], board.turn_info.last_move_dst[1], state,
			fields[0], fields[1], fields[2], fields[3],
			fields[4], fields[5], fields[6], fields[7],
			fields[8], fields[9], fields[10], fields[11],
			fields[12], fields[13], fields[14], fields[15],
			fields[16], fields[17]
	);
 }
 
/******************************************************************************
 * Transforms a GameInfo struct into its JSON equivalent.
 *****************************************************************************/
 int game_info_to_json(GameInfo info, char *json)
 {
	char en_passant[4];	/* [yes|no] */

	/* change int value into string */
	int err = set_yes_no(info.en_passant, en_passant);
	if (err < 0)
	{
		return -1;
	}

	/* fill json string from values in struct */
	return sprintf(json,
			"{\"player1\": %s, \"player2\": %s, \"round\": %s, \"location\": %s, \"enPassant\": %s, \"timerFormat\": {\"time\": %d, \"increment\": %d, \"limit\": %d, \"overtime\": %d, \"overtimeIncrement\": %d}}",
			info.player_1, info.player_2, info.round, info.location, en_passant,
			info.timer_format.time, info.timer_format.increment, info.timer_format.limit,
			info.timer_format.overtime, info.timer_format.overtime_increment
	);

 }
