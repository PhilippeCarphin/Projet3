#include <string.h>
#include <stdio.h>

#include "jsmn.h"
#include "json_tokens.h"
#include  "chessboard_rest_protocol.h"

/******************************************************************************
 * Creates a game_info structure from a JSON formatted string.
 *****************************************************************************/
int parse_game_info(const char *data, GameInfo *gameinfo)
{
	const int MAX_TOKENS = 128;
	int i, number_of_tokens;
	jsmn_parser parser;
	jsmntok_t tokens[MAX_TOKENS];
	GameInfo info;
	
	jsmn_init(&parser);
	number_of_tokens = jsmn_parse(&parser, data, strlen(data), tokens, sizeof(tokens)/sizeof(tokens[0]));
		
	/* Assume the top-level element is an object */
	if (number_of_tokens < 0)
		return -1;
	if (number_of_tokens < 1 || tokens[0].type != JSMN_OBJECT)
		return -1;
	
	/* Loop over all keys of the root object */
	for (i = 1; i < number_of_tokens; i+=2)
	{
		if (jsoneq(data, &tokens[i], "player1") == 0)
		{
			if (token_to_string(info.player_1, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "player2") == 0)
		{			
			if (token_to_string(info.player_2, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "round") == 0)
		{			
			if (token_to_string(info.round, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "location") == 0)
		{
			if (token_to_string(info.location, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "secret_code") == 0)
		{
			if (token_to_string(info.secret_code, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "twoTablet") == 0)
		{
			if (token_to_bool(&info.two_tablet, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "enPassant") == 0)
		{			
			if (token_to_bool(&info.en_passant, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "timerFormat") == 0)
		{
			int j;
			if (tokens[i+1].type != JSMN_OBJECT) 
			{
				continue; /* We expect timer to be an object */
			}
			
			for (j = 2; j < tokens[i+1].size*2+1; j+=2)
			{
				if (jsoneq(data, &tokens[i+j], "time") == 0)
				{
					if (token_to_int(&info.timer_format.time, data, &tokens[i+j+1]) < 0)
					{
						return -1;
					}
				}
				else if (jsoneq(data, &tokens[i+j], "increment") == 0)
				{
					if (token_to_int(&info.timer_format.increment, data, &tokens[i+j+1]) < 0)
					{
						return -1;
					}
				}
				else if (jsoneq(data, &tokens[i+j], "limit") == 0)
				{
					if (token_to_int(&info.timer_format.limit, data, &tokens[i+j+1]) < 0)
					{
						return -1;
					}
				}
				else if (jsoneq(data, &tokens[i+j], "overtime") == 0)
				{
					if (token_to_int(&info.timer_format.overtime, data, &tokens[i+j+1]) < 0)
					{
						return -1;
					}
				}
				else if (jsoneq(data, &tokens[i+j], "overtimeIncrement") == 0)
				{
					if (token_to_int(&info.timer_format.overtime_increment, data, &tokens[i+j+1]) < 0)
					{
						return -1;
					}
				}
				else
				{
					return -1;
				}
			}
			i += tokens[i+1].size*2;
		}
		else
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
	const int MAX_TOKENS = 128;
	int i, number_of_tokens, number_of_pieces;
	jsmn_parser parser;
	jsmntok_t tokens[MAX_TOKENS];
	BoardPosition board;
	
	jsmn_init(&parser);
	number_of_tokens = jsmn_parse(&parser, data, strlen(data), tokens, sizeof(tokens)/sizeof(tokens[0]));
		
	/* Assume the top-level element is an object */
	if (number_of_tokens < 0)
		return -1;
	if (number_of_tokens < 1 || tokens[0].type != JSMN_OBJECT)
		return -1;
	
	/* Loop over all keys of the root object */
	for (i = 1, number_of_pieces = 0; i < number_of_tokens; i+=2)
	{
		if (jsoneq(data, &tokens[i], "turn") == 0)
		{
			if (token_to_int(&board.turn, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "move_no") == 0)
		{			
			if (token_to_int(&board.move_no, data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "king1") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "queen1") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "bishop1A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "bishop1B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "rook1A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "rook1B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "knight1A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "knight1B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "pawn1") == 0)
		{			
			int j;
			if (tokens[i+1].type != JSMN_ARRAY) 
			{
				continue; /* We expect timer to be an object */
			}
			for (j = 0; j < tokens[i+1].size; j++) 
			{
				jsmntok_t *subtokens = &tokens[j+1];
				if (token_to_string(board.positions[number_of_pieces++], data, &subtokens[i+1]) < 0)
				{
					return -1;
				}
			}
			i += tokens[i+1].size;
		}
		else if (jsoneq(data, &tokens[i], "king2") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "queen2") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "bishop2A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "bishop2B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "rook2A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "rook2B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "knight2A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "knight2B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return -1;
			}
		}
		else if (jsoneq(data, &tokens[i], "pawn2") == 0)
		{			
			int j;
			if (tokens[i+1].type != JSMN_ARRAY) 
			{
				continue; /* We expect timer to be an object */
			}
			for (j = 0; j < tokens[i+1].size; j++) 
			{
				jsmntok_t *subtokens = &tokens[j+1];
				if (token_to_string(board.positions[number_of_pieces++], data, &subtokens[i+1]) < 0)
				{
					return -1;
				}
			}
			i += tokens[i+1].size;
		}
	}
	
	*boardpos = board;
	return 0;
}
