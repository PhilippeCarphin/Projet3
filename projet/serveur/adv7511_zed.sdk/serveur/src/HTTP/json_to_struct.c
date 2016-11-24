#include "json_to_struct.h"
#include "json_tokens.h"
#include "xil_io.h"

static const int MAX_TOKENS = 128;

/******************************************************************************
 * Creates a game_info structure from a JSON formatted string.
 *****************************************************************************/
int parse_game_info(const char *data, GameInfo *gameinfo)
{
	int number_of_tokens;			/* number of tokens found when parsed */
	GameInfo info;					/* temporary struct to be filled */
	jsmn_parser parser;				/* parser, provided by jsmn */
	jsmntok_t tokens[MAX_TOKENS];	/* tokens, split correctly by jsmn */
	
	jsmn_init(&parser);
	number_of_tokens = jsmn_parse(&parser, data, strlen(data), tokens, sizeof(tokens)/sizeof(tokens[0]));
		
	/* Assume the top-level element is an object */
	if (number_of_tokens < 0)
	{
		return -1;
	}
	if (number_of_tokens < 1 || tokens[0].type != JSMN_OBJECT)
	{
		return -1;
	}
	
	/*
	 *  Loop over all keys of the root object.
	 *  For each loop:
	 *  	tokens[i] contains the field name
	 *  	tokens[i+1] contains the field value
	 */
	int i, status;
	for (i = 1; i < number_of_tokens; i+=2)
	{
		if (jsoneq(data, &tokens[i], "player1") == 0)
		{
			status = token_to_string(info.player_1, data, &tokens[i+1]);
		}
		else if (jsoneq(data, &tokens[i], "player2") == 0)
		{			
			status = token_to_string(info.player_2, data, &tokens[i+1]);
		}
		else if (jsoneq(data, &tokens[i], "round") == 0)
		{			
			status = token_to_string(info.round, data, &tokens[i+1]);
		}
		else if (jsoneq(data, &tokens[i], "location") == 0)
		{
			status = token_to_string(info.location, data, &tokens[i+1]);
		}
		else if (jsoneq(data, &tokens[i], "secret_code") == 0)
		{
			status = token_to_string(info.secret_code, data, &tokens[i+1]);
		}
		else if (jsoneq(data, &tokens[i], "twoTablet") == 0)
		{
			status = token_to_bool(&info.two_tablet, data, &tokens[i+1]);
		}
		else if (jsoneq(data, &tokens[i], "enPassant") == 0)
		{			
			status = token_to_bool(&info.en_passant, data, &tokens[i+1]);
		}
		else if (jsoneq(data, &tokens[i], "timerFormat") == 0)	/* timerFormat is itself a JSMN_OBJECT */
		{
			if (tokens[i+1].type != JSMN_OBJECT) 
			{
				continue; /* We expect timer to be an object */
			}
			
			/*
			 *  Loop over all keys of the sub-object.
			 *  For each loop:
			 *  	tokens[i+j] contains the field name
			 *  	tokens[i+j+1] contains the field value
			 */
			int j;
			for (j = 2; j < tokens[i+1].size*2+1; j+=2)
			{
				if (jsoneq(data, &tokens[i+j], "time") == 0)
				{
					status = token_to_int(&info.timer_format.time, data, &tokens[i+j+1]);
				}
				else if (jsoneq(data, &tokens[i+j], "increment") == 0)
				{
					status = token_to_int(&info.timer_format.increment, data, &tokens[i+j+1]);
				}
				else if (jsoneq(data, &tokens[i+j], "limit") == 0)
				{
					status = token_to_int(&info.timer_format.limit, data, &tokens[i+j+1]);
				}
				else if (jsoneq(data, &tokens[i+j], "overtime") == 0)
				{
					status = token_to_int(&info.timer_format.overtime, data, &tokens[i+j+1]);
				}
				else if (jsoneq(data, &tokens[i+j], "overtimeIncrement") == 0)
				{
					status = token_to_int(&info.timer_format.overtime_increment, data, &tokens[i+j+1]);
				}
				else /* failed to identify token */
				{
					return -1;
				}

				/* error checking */
				if (status < 0)
				{
					return -1;
				}
			}
			i += tokens[i+1].size*2;	/* set i to after the timer object */
		}
		else /* failed to identify token */
		{
			return -1;
		}

		/* error checking */
		if (status < 0)
		{
			return -1;
		}
	}
	
	*gameinfo = info;
	return 0;
}

/******************************************************************************
* Creates a board_position structure from a JSON formatted string.
 *****************************************************************************/
 int parse_board_position(const char *data, BoardPosition *boardpos)
{
	int number_of_tokens;			/* number of tokens found when parsed */
	int number_of_pieces;			/* number of pieces found into the JSON string */
	BoardPosition board;			/* temporary struct to be filled */
	jsmn_parser parser;				/* parser, provided by jsmn */
	jsmntok_t tokens[MAX_TOKENS];	/* tokens, split correctly by jsmn */

	/* non-pawn pieces names */
#if 0 // Unused variable compiler warning
	static const char pieces_names[][20] =
	{
		"king1", "queen1", "bishop1A", "bishop1B", "rook1A", "rook1B", "knight1A", "knight1B",
		"king2", "queen2", "bishop2A", "bishop2B", "rook2A", "rook2B", "knight2A", "knight2B"
	};
#endif
	
	jsmn_init(&parser);
	number_of_tokens = jsmn_parse(&parser, data, strlen(data), tokens, sizeof(tokens)/sizeof(tokens[0]));

	/* Assume the top-level element is an object */
	if (number_of_tokens < 0)
	{
		return -1;
	}
	if (number_of_tokens < 1 || tokens[0].type != JSMN_OBJECT)
	{
		return -1;
	}

	/*
	 *  Loop over all keys of the root object.
	 *  For each loop:
	 *  	tokens[i] contains the field name
	 *  	tokens[i+1] contains the field value
	 *  	number_of_pieces is incremented when a piece is found
	 */
	int i, status;
	for (i = 1, number_of_pieces = 0; i < number_of_tokens; i+=2)
	{
		if (jsoneq(data, &tokens[i], "turn") == 0)
		{
			status = token_to_int(&board.turn_info.turn, data, &tokens[i+1]);
		}
		else if (jsoneq(data, &tokens[i], "move_no") == 0)
		{			
			status = token_to_int(&board.turn_info.move_no, data, &tokens[i+1]);
		}
		/* same process for all pieces but the pawns */
		else if(jsoneq(data, &tokens[i], "king1") == 0
			||  jsoneq(data, &tokens[i], "queen1") == 0
			||	jsoneq(data, &tokens[i], "bishop1A") == 0
			||	jsoneq(data, &tokens[i], "bishop1B") == 0
			||	jsoneq(data, &tokens[i], "rook1A") == 0
			||	jsoneq(data, &tokens[i], "rook1B") == 0
			||	jsoneq(data, &tokens[i], "knight1A") == 0
			||	jsoneq(data, &tokens[i], "knight1B") == 0
			||	jsoneq(data, &tokens[i], "king2") == 0
			||  jsoneq(data, &tokens[i], "queen2") == 0
			||	jsoneq(data, &tokens[i], "bishop2A") == 0
			||	jsoneq(data, &tokens[i], "bishop2B") == 0
			||	jsoneq(data, &tokens[i], "rook2A") == 0
			||	jsoneq(data, &tokens[i], "rook2B") == 0
			||	jsoneq(data, &tokens[i], "knight2A") == 0
			||	jsoneq(data, &tokens[i], "knight2B") == 0)
		{			
			status = token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]);
		}
		/* same process for pawns */
		else if(jsoneq(data, &tokens[i], "pawn1") == 0	/* pawn1 is a JSMN_ARRAY */
			||	jsoneq(data, &tokens[i], "pawn2") == 0) /* pawn2 is a JSMN_ARRAY */
		{			
			if (tokens[i+1].type != JSMN_ARRAY)
			{
				continue; /* We expect timer to be an object */
			}

			/*
			 *  Loop over all keys of the array.
			 *  For each loop:
			 *  	tokens[j+1] contains the field name
			 *  	subtokens[i+1] contains the field value
			 */
			int j;
			for (j = 0; j < tokens[i+1].size; j++) 
			{
				jsmntok_t *subtokens = &tokens[j+1];
				status = token_to_string(board.positions[number_of_pieces++], data, &subtokens[i+1]);

				/* error checking */
				if (status < 0)
				{
					return -1;
				}
			}
			i += tokens[i+1].size;	/* set i to after the array */
		}
		else /* failed to identify token */
		{
			return -1;
		}

		/* error checking */
		if (status < 0)
		{
			return -1;
		}
	}
	
	*boardpos = board;
	return 0;
}
