
//ifdef chose

#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_
#include "chessboard_rest_protocol.h"
#include "PieceEnum.h"
typedef enum { false = 0 , true = 1 } bool;

enum ChessboardRestStatus new_game(GameInfo *gameInfo);					/* return 200 if ok */
enum ChessboardRestStatus movePiece(int player, const char *src, const char *dst, MoveInfo* moveInfo);
enum ChessboardRestStatus promote_piece(int player, const char *new_type);	/* return 200 if ok */
enum ChessboardRestStatus get_time(int player,TimeInfo *timeInfo);
enum ChessboardRestStatus get_summary(TurnInfo *turnInfo);
enum ChessboardRestStatus get_board(BoardPosition *boardPosition);
enum ChessboardRestStatus set_board(BoardPosition *boardPosition);			/* return 200 if ok */
enum ChessboardRestStatus get_game_info(GameInfo *gameInfo);
enum ChessboardRestStatus start_game();										/* return 200 if ok */
enum ChessboardRestStatus end_game();	
enum ChessboardRestStatus validate_password(const char *pswd);
typedef struct Piece
{
	PieceType pieceType;
	bool alive;
	int x;
	int y;
	int rock;
	bool enPassant; // TODO: could become a bool. Should be set after the permitted jump and unset the turn after.
	PlayerID playerID;
}Piece;

#endif /* CHESSBOARD_H_ */
