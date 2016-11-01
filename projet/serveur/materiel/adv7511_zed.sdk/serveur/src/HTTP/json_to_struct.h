/*
 * json_to_struct.h
 */

#ifndef JSON_TO_STRUCT_H_
#define JSON_TO_STRUCT_H_

int parse_game_info(const char *data, GameInfo *gameinfo);
int parse_board_position(const char *data, BoardPosition *boardpos);

#endif /* JSON_TO_STRUCT_H_ */
