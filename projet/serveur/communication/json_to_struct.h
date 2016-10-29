/*
 * json_to_struct.h
 */

#ifndef JSON_TO_STRUCT_H_
#define JSON_TO_STRUCT_H_

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

struct game_info *parse_game_info(const char *data);
struct board_position *parse_board_position(const char *data);

#endif /* JSON_TO_STRUCT_H_ */
