#include <string.h>
#include <stdio.h>

#include "jsmn.h"
#include "json_tokens.h"

#define MAX_STR_LENGTH 20

struct game_info {
	char player_1[MAX_STR_LENGTH];
	char player_2[MAX_STR_LENGTH];
	char round[MAX_STR_LENGTH];
	char location[MAX_STR_LENGTH];
	char secret_code[MAX_STR_LENGTH];
	int two_tablet;
	int en_passant;
	struct {
		int time;
		int increment;
		int limit;
		int overtime;
		int overtime_increment;
	} timer_format;
};

struct board_position {
	int turn;
	int move_no;
	char positions[32][3];
};

/******************************************************************************
 * Creates a game_info structure from a JSON formatted string.
 *****************************************************************************/
struct game_info *parse_game_info(const char *data)
{
	const int MAX_TOKENS = 128;
	int i, number_of_tokens;
	jsmn_parser parser;
	jsmntok_t tokens[MAX_TOKENS];
	static struct game_info info;
	
	jsmn_init(&parser);
	number_of_tokens = jsmn_parse(&parser, data, strlen(data), tokens, sizeof(tokens)/sizeof(tokens[0]));
		
	/* Assume the top-level element is an object */
	if (number_of_tokens < 0)
		return NULL;	
	if (number_of_tokens < 1 || tokens[0].type != JSMN_OBJECT)
		return NULL;
	
	/* Loop over all keys of the root object */
	for (i = 1; i < number_of_tokens; i+=2)
	{
		if (jsoneq(data, &tokens[i], "player1") == 0)
		{
			if (token_to_string(info.player_1, data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "player2") == 0)
		{			
			if (token_to_string(info.player_2, data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "round") == 0)
		{			
			if (token_to_string(info.round, data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "location") == 0)
		{
			if (token_to_string(info.location, data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "secret_code") == 0)
		{
			if (token_to_string(info.secret_code, data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "twoTablet") == 0)
		{
			if (token_to_bool(&info.two_tablet, data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "enPassant") == 0)
		{			
			if (token_to_bool(&info.en_passant, data, &tokens[i+1]) < 0)
			{
				return NULL;
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
						return NULL;
					}
				}
				else if (jsoneq(data, &tokens[i+j], "increment") == 0)
				{
					if (token_to_int(&info.timer_format.increment, data, &tokens[i+j+1]) < 0)
					{
						return NULL;
					}
				}
				else if (jsoneq(data, &tokens[i+j], "limit") == 0)
				{
					if (token_to_int(&info.timer_format.limit, data, &tokens[i+j+1]) < 0)
					{
						return NULL;
					}
				}
				else if (jsoneq(data, &tokens[i+j], "overtime") == 0)
				{
					if (token_to_int(&info.timer_format.overtime, data, &tokens[i+j+1]) < 0)
					{
						return NULL;
					}
				}
				else if (jsoneq(data, &tokens[i+j], "overtimeIncrement") == 0)
				{
					if (token_to_int(&info.timer_format.overtime_increment, data, &tokens[i+j+1]) < 0)
					{
						return NULL;
					}
				}
				else
				{
					return NULL;
				}
			}
			i += tokens[i+1].size*2;
		}
		else
		{
			return NULL;
		}
	}
	
	return &info;
}

/******************************************************************************
* Creates a board_position structure from a JSON formatted string.
 *****************************************************************************/
struct board_position *parse_board_position(const char *data)
{
	const int MAX_TOKENS = 128;
	int i, number_of_tokens, number_of_pieces;
	jsmn_parser parser;
	jsmntok_t tokens[MAX_TOKENS];
	static struct board_position board;
	
	jsmn_init(&parser);
	number_of_tokens = jsmn_parse(&parser, data, strlen(data), tokens, sizeof(tokens)/sizeof(tokens[0]));
		
	/* Assume the top-level element is an object */
	if (number_of_tokens < 0)
		return NULL;	
	if (number_of_tokens < 1 || tokens[0].type != JSMN_OBJECT)
		return NULL;
	
	/* Loop over all keys of the root object */
	for (i = 1, number_of_pieces = 0; i < number_of_tokens; i+=2)
	{
		if (jsoneq(data, &tokens[i], "turn") == 0)
		{
			if (token_to_int(&board.turn, data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "move_no") == 0)
		{			
			if (token_to_int(&board.move_no, data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "king1") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "queen1") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "bishop1A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "bishop1B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "rook1A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "rook1B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "knight1A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "knight1B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
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
					return NULL;
				}
			}
			i += tokens[i+1].size;
		}
		else if (jsoneq(data, &tokens[i], "king2") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "queen2") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "bishop2A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "bishop2B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "rook2A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "rook2B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "knight2A") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
			}
		}
		else if (jsoneq(data, &tokens[i], "knight2B") == 0)
		{			
			if (token_to_string(board.positions[number_of_pieces++], data, &tokens[i+1]) < 0)
			{
				return NULL;
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
					return NULL;
				}
			}
			i += tokens[i+1].size;
		}
	}
	
	return &board;
}