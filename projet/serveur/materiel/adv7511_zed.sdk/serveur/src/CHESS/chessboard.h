#include "chessboard_rest_protocol.h"
//ifdef chose

#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

enum ChessboardRestStatus new_game(GameInfo *gameInfo);					/* return 200 if ok */
enum ChessboardRestStatus move_piece(int player, const char *src, const char *dst, MoveInfo* moveInfo);
enum ChessboardRestStatus promote_piece(int player, const char *new_type);	/* return 200 if ok */
enum ChessboardRestStatus get_time(int player,TimeInfo *timeInfo);
enum ChessboardRestStatus get_summary(TurnInfo *turnInfo);
enum ChessboardRestStatus get_board(BoardPosition *boardPosition);
enum ChessboardRestStatus set_board(BoardPosition *boardPosition);			/* return 200 if ok */
enum ChessboardRestStatus get_game_info(GameInfo *gameInfo);
enum ChessboardRestStatus start_game();										/* return 200 if ok */
enum ChessboardRestStatus end_game();	

#endif /* CHESSBOARD_H_ */
