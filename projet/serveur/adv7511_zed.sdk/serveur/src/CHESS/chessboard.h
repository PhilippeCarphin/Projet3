#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include "chessboard_rest_protocol.h"
#include "PieceEnum.h"


/* simple c boolean implementation */
typedef enum { false = 0 , true = 1 } bool;

/******************************************************************************
 * This struct is an internal representation of a chess piece.
 * While it knows its position (x,y), the Chessboard module stores its pieces
 * into a 2-dimensionnal array of pointers, called the board.
 *
 * Since the board's indexes represent the pieces' position, it will change
 * the position of the pointers and even clear some indexes when needed.
 *
 * Therefore, the module has two 1-dimensionnal arrays containing the same
 * pointers, without the dynamic changes of positions.
 ******************************************************************************/
typedef struct Piece
{
	PieceType pieceType;
	bool alive;
	int x;					/* coordinates */
	int y;					/* coordinates */
	int rock;				/* TODO: has_moved instead of rock; used on king and rook */
	/* Indicates that the piece can be captured on the next turn.
	 * Should be set after the permitted jump and unset the turn after. */
	bool enPassant;
	PlayerID playerID;
}Piece;

Piece player1Pieces[16];
Piece player2Pieces[16];

enum moveResult{
	VALID,
	ILLEGAL,
	ENPASSANT,
	CASTLING
};

/******************************************************************************
 * Copy the received game informations into internal structure and
 * initialize the game. Display the chessboard in its standard state.
 ******************************************************************************/
enum ChessboardRestStatus new_game(GameInfo *gameInfo);
void reset_game();

/******************************************************************************
 * Set gameStarted to true and inform the HDMI module.
 ******************************************************************************/
enum ChessboardRestStatus start_game();
enum ChessboardRestStatus join_game();

/******************************************************************************
 * Return to initial state: set gameStarted to false and display the
 * welcome screen.
******************************************************************************/
enum ChessboardRestStatus end_game();
void force_end_game();

/******************************************************************************
 * Set the chessboard to a custom piece configuration.
 * Immediately display on screen.
 ******************************************************************************/
enum ChessboardRestStatus set_board(BoardPosition *boardPosition);

/******************************************************************************
 * Valid a player's move and, if successful, updates the chessboard accordingly.
 * Display the change on screen.
 *
 * This is the logic core of the program, where most of the actual decisions
 * are made.
 ******************************************************************************/
enum ChessboardRestStatus movePiece(int player, const char *src, const char *dst, MoveInfo* moveInfo);
enum moveResult execute_move(Piece *piece, int xs, int xd, int ys, int yd);

/******************************************************************************
 * Change a pawn piece's type. The usual checks are made
 * (right player, old type is valid, new type is valid).
 ******************************************************************************/
enum ChessboardRestStatus promote_piece(int player, const char *new_type);

/******************************************************************************
 * Compare the given password with the internal game password.
 * Should be called by REST module before most of its requests.
 ******************************************************************************/
enum ChessboardRestStatus validate_password(const char *pswd);

/******************************************************************************
 * Give information about the turn
 * (who's turn it is, move number, last move, game status).
 ******************************************************************************/
enum ChessboardRestStatus get_summary(TurnInfo *turnInfo);

/******************************************************************************
 * Give game's details, as configured by a /new_game request.
 ******************************************************************************/
enum ChessboardRestStatus get_game_info(GameInfo *gameInfo);

/******************************************************************************
 * Give the position and aliveness of each piece of the chessboard.
 * Also gives the turn and move number.
 ******************************************************************************/
enum ChessboardRestStatus get_board(BoardPosition *boardPosition);

/******************************************************************************
 * Give a player its time informations (time remaining, overtime).
 ******************************************************************************/
enum ChessboardRestStatus get_time(int player,TimeInfo *timeInfo);

#endif /* CHESSBOARD_H_ */
