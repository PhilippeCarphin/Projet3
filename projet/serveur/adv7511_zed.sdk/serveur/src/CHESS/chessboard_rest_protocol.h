//ajouter les ifdef indef

#ifndef CHESSBOARD_REST_PROTOCOL_H_
#define CHESSBOARD_REST_PROTOCOL_H_

#define MAX_STR_LENGTH 1024
typedef enum {player1 = 1, player2 = 2} PlayerID;

enum ChessboardRestStatus {OK = 200, unathorized = 401, gameInProgress = 403, notYourTurn = 403, deplacementIllegal = 406, tempsEcoule = 408, NOT_IMPLEMENTED = -1, INTERNAL_ERROR = 500 };
enum State {NORMAL, CHECKMATE, CHECK, STALEMATE, RESTARTED};

typedef struct GameInfo {
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
	} timer_format;				/* see ExigencesTechniques for details */

}GameInfo;

typedef struct MoveInfo {
	char piece_eliminated[2];	/* position of eliminated, or X */
	int promotion;				/* 1 if promoted pawn, else 0 */	
	enum State game_status;			/* NORMAL|CHECKMATE|CHECK|STALEMATE */
}MoveInfo;

typedef struct TimeInfo {
	int time;
	int overtime;
}TimeInfo;

typedef struct TurnInfo {
	PlayerID turn; 			/* player 1 or 2 */
	int move_no;
	char last_move_src[2];	/* position of last move, or X if first turn */
	char last_move_dst[2];	/* position of last move, or X if first turn */
	enum State game_status;	/* NORMAL|CHECKMATE|CHECK|STALEMATE|RESTARTED */
}TurnInfo;

typedef struct BoardPosition {
	//int turn;
	//int move_no;
	TurnInfo turn_info;
	char positions[32][2];	/* 32 pieces, 2 char per position */
}BoardPosition;
#endif /* CHESSBOARD_REST_PROTOCOL_H_ */


