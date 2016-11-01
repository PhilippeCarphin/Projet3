/*
 * json_from_struct.h
 */

#ifndef JSON_FROM_STRUCT_H_
#define JSON_FROM_STRUCT_H_

#include "chessboard_rest_protocol.h"

int move_info_to_json(MoveInfo info, char *json);
int time_info_to_json(TimeInfo info, char *json);
int turn_info_to_json(TurnInfo info, char *json);
int board_position_to_json(BoardPosition board, char *json);
int game_info_to_json(GameInfo info, char *json);

#endif /* JSON_FROM_STRUCT_H_ */
