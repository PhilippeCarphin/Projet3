/*
 * json_from_struct.h
 */

#ifndef JSON_FROM_STRUCT_H_
#define JSON_FROM_STRUCT_H_

int move_info_to_json(/*struct move_info info, char *json */);
int time_info_to_json(/*struct time_info info, char *json */);
int turn_info_to_json(/*struct turn_info info, char *json */);
int board_position_to_json(/*struct board_position info, char *json */);
int game_info_to_json(/*struct game_info info*, char *json */);

#endif /* JSON_FROM_STRUCT_H_ */
