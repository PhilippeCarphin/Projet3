#include "chessboard.h"
#include "BoardDisplay.h"
#include "chessclock.h"
#include "ZedBoardConfig.h"
// #define DEBUG
#include "debug.h"

#include <string.h>
#include "xil_io.h"

/******************************************************************************
 * Structures and global variables
 ******************************************************************************/
static GameInfo currentGameInfo;
TurnInfo currentTurnInfo;

static Piece* boardGame[8][8];

static bool gameStarted = false;

/******************************************************************************
 * Declarations of internal functions
 ******************************************************************************/
static void ChessGameInitialisation();
static Piece PieceInitialisation(int x, int y,PieceType type, PlayerID playerID);
static void setBoard(Piece* playerPieces);
static void clearBoard();

static bool can_promote(Piece *piece);
static PieceType get_type(const char *type_name);
static int find_available_type(PieceType type, int player);
static void toggle_next_turn();
static void clear_enPassant(int player);

enum moveResult execute_move(Piece *piece, int xs, int xd, int ys, int yd);
static enum moveResult move_king(int xs, int xd, int ys, int yd);
static enum moveResult move_rook(int xs, int xd, int ys, int yd);
static enum moveResult move_bishop(int xs, int xd, int ys, int yd);
static enum moveResult move_knight(int xs, int xd, int ys, int yd);
static enum moveResult move_queen(int xs, int xd, int ys, int yd);
static enum moveResult move_pawn(int xs, int xd, int ys, int yd);


/******************************************************************************
 * Copy the received game informations into internal structure and
 * initialize the game. Display the chessboard in its standard state.
 ******************************************************************************/
enum ChessboardRestStatus new_game(GameInfo *gameInfo)
{
	if (gameStarted)
	{
		return gameInProgress;
	}
	else
	{
		currentGameInfo = *gameInfo;
		ChessGameInitialisation();
		xil_printf("\n======== Game Started ========\n");
		BoardDisplay_new_board(&currentGameInfo);
		return OK;
	}

}

/******************************************************************************
 * Put the chessboard in the state it would be just after a /new_game, without
 * reseting the game_info struct.
 ******************************************************************************/
void reset_game()
{
	ChessGameInitialisation();
	currentTurnInfo.game_status = RESTARTED;	// Important for client
	xil_printf("\n======== Game Restarted ========\n");
	BoardDisplay_new_board(&currentGameInfo);
	gameStarted = false;
	end_game_led();
}

/******************************************************************************
 * Set gameStarted to true and inform the HDMI module.
 ******************************************************************************/
enum ChessboardRestStatus start_game()
{
	if (gameStarted)
	{
		return gameInProgress;
	}

	gameStarted = true;
	currentTurnInfo.game_status = NORMAL;	// In case it was just restarted

	BoardDisplay_start_game();
	start_game_led();
	return OK;
}

/******************************************************************************
 * Called when a 2nd player wants to join game. Since password is already checked
 * earlier, we can just accept at this point.
 ******************************************************************************/
enum ChessboardRestStatus join_game()
{
	return OK;
}

/******************************************************************************
 * Return to initial state: sets gameStarted to false and display the
 * welcome screen.
******************************************************************************/
enum ChessboardRestStatus end_game()
{
	FBEGIN;
	if (!gameStarted)
	{
		return unathorized;
	}
	force_end_game();
	return OK;
}

/******************************************************************************
 * Return to initial state; Does not check wether game is started, and returns
 * nothing. Should not fail.
******************************************************************************/
void force_end_game()
{
	gameStarted = false;
	currentGameInfo.secret_code[0] = '\0';
	xil_printf("\n===== Game Ended ========\n");
	BoardDisplay_welcome_screen();
	end_game_led();
}

/******************************************************************************
 * Set the chessboard to a custom piece configuration.
 * Immediately display on screen.
 ******************************************************************************/
enum ChessboardRestStatus set_board(BoardPosition *boardPosition)
{
	/* clear the board */
	ChessGameInitialisation();
	clearBoard();

	/* set player 1 pieces:
	 * 	- their aliveness
	 * 	- their position (if alive)
	 */
	int i= 0;
	for (i = 0; i < 16; i++)
	{
		if (boardPosition->positions[i][0] == 'x')
		{
			player1Pieces[i].alive = false;
		}
		else
		{
			player1Pieces[i].alive = true;
			player1Pieces[i].x  = boardPosition->positions[i][0] - 'a';
			player1Pieces[i].y  = boardPosition->positions[i][1] - '1';
		}

	}

	/* set player 2 pieces (same as with player 1) */
	for (i = 16; i < 32; i++)
	{
		if (boardPosition->positions[i][0] == 'x')
		{
			player2Pieces[i-16].alive = false;
		}
		else
		{
			player2Pieces[i-16].alive = true;
			player2Pieces[i-16].x = boardPosition->positions[i][0] - 'a';
			player2Pieces[i-16].y = boardPosition->positions[i][1] - '1';
		}
	}


	/* set the board with the now set pieces
	 * (set individually for each player)
	 */
	setBoard(player1Pieces);
	setBoard(player2Pieces);

	/* initialize run informations */
	currentTurnInfo = boardPosition->turn_info;

	/* display newly set board on HDMI */
	if (BoardDisplay_draw_pieces_custom(player1Pieces,player2Pieces) != 0)
	{
		xil_printf("Error in HDMI restSetBoard");
	}
	return OK;

}

/******************************************************************************
 * Valid a player's move and, if successful, updates the chessboard accordingly.
 * Display the change on screen.
 *
 * This is the logic core of the program, where most of the actual decisions
 * are made.
 ******************************************************************************/
enum ChessboardRestStatus movePiece(int player, const char *src, const char *dst, MoveInfo* moveInfo)
{
	FBEGIN;
	struct Move mv; 		// infos for BoardDisplay/HDMI
	mv.enPassant = false;	// set to true only if a piece is captured by En Passant
	moveInfo->piece_eliminated[0] = 'x';
	moveInfo->piece_eliminated[1] = 'x'; // will be overwritten if a piece is eliminated.

	clear_enPassant(player);

	// check if it's the player turn
	if (player != currentTurnInfo.turn)
	{
		return notYourTurn;
	}

	// extract positions and pieces
	int xs = src[0]-'a'; // x of source position
	int ys = src[1]-'1'; // y of source position
	int xd = dst[0]-'a'; // x of destination
	int yd = dst[1]-'1'; // y of destination

	// Look for easy-to-find illegal moves.
	if (xs<0 || xs>7 || ys<0 || ys>7 || xd<0 || xd>7 || yd<0 || yd>7)
		return deplacementIllegal; // out of the board
	if (boardGame[xs][ys] == 0)
		return deplacementIllegal; // no piece here
	if (xs == xd && ys == yd)
		return deplacementIllegal; // not moving
	if (boardGame[xs][ys]->playerID != player)
		return deplacementIllegal; // opponent piece

	// Ok, ok, seems fine.
	// Now, look for piece-specific illegal moves.
	// Find out if there is a castling or en passant situation, while you're at it.
	Piece *piece = boardGame[xs][ys];
	switch(execute_move(piece, xs, xd, ys, yd))
	{
	case ILLEGAL:
		return deplacementIllegal;

	case CASTLING:
		return NOT_IMPLEMENTED;

	case ENPASSANT:
		boardGame[xd][ys]->alive = false; // special capture using ys instead of yd +/- 1
		boardGame[xd][ys] = NULL; // special cleaning

		// specify which piece is eliminated
		moveInfo->piece_eliminated[0] = xd + 'a';
		moveInfo->piece_eliminated[1] = ys + '1';

		// tell HDMI that a piece was captured through en passant
		mv.enPassant = true;
		break;

	case VALID:
		break;

	default:
		return NOT_IMPLEMENTED;
	}

	// If we made it this far, the move is valid (for now).
	if (boardGame[xd][yd] != NULL)	// a piece is captured
	{
		if (boardGame[xd][yd]->playerID == player)
		{
			return deplacementIllegal; // capturing allied piece
		}
		boardGame[xd][yd]->alive = false; // capture enemy piece
		moveInfo->piece_eliminated[0] = xd + 'a';
		moveInfo->piece_eliminated[1] = yd + '1';
	}

	// TODO: check for check, checkmate, stalemate

	// ACTUALLY move the piece
	boardGame[xd][yd] = piece; // move the piece
	boardGame[xs][ys] = 0; // clear the source space
	boardGame[xd][yd]->x = xd;
	boardGame[xd][yd]->y = yd;

	// check for promotion
	moveInfo->promotion = can_promote(piece);

	// increment turn, change player turn, time stuff
	if (moveInfo->promotion == false)
	{
		// turn not done yet; piece must be promoted first
		toggle_next_turn();
	}
	currentTurnInfo.last_move_src[0] = xs + 'a';
	currentTurnInfo.last_move_src[1] = ys + '1';
	currentTurnInfo.last_move_dst[0] = xd + 'a';
	currentTurnInfo.last_move_dst[1] = yd + '1';

	// TODO: change these values according to state
	currentTurnInfo.game_status = NORMAL;
	moveInfo->game_status = NORMAL;

	//call HDMI draw functions
	mv.t = boardGame[xd][yd]->pieceType;
	mv.c = (boardGame[xd][yd]->playerID == player1) ? WHITE : BLACK;
	mv.o_file = xs;
	mv.o_rank = ys;
	mv.d_file = xd;
	mv.d_rank = yd;
	mv.turn_number = currentTurnInfo.move_no - 1;
	mv.capture = (moveInfo->piece_eliminated[0] == 'x') ? 0 : 1;

	BoardDisplay_move_piece(&mv);

	FEND;
	return OK;
}

/******************************************************************************
 * Change a pawn piece's type. The usual checks are made
 * (right player, old type is valid, new type is valid).
 ******************************************************************************/
enum ChessboardRestStatus promote_piece(int player, const char *new_type)
{
	/* check if it's the player turn */
	if (player != currentTurnInfo.turn)
	{
		return notYourTurn;
	}

	/* retreive piece */
	int x = currentTurnInfo.last_move_dst[0] - 'a';
	int y = currentTurnInfo.last_move_dst[1] - '1';
	Piece *piece = boardGame[x][y];
	Piece *playerPieces = (player == 1 ? player1Pieces : player2Pieces);

	/* set new piece type */
	PieceType type = get_type(new_type);
	if (can_promote(piece) == true && type >= 0 && type != PAWN)
	{
		/* check if there is space for asked type */
		int index = find_available_type(type, player);
		if (index < 0)
			return unathorized;

		/* kill the pawn, revive the other piece and places it at its new space */
		piece->alive = false;
		playerPieces[index].alive = true;
		playerPieces[index].x = x;
		playerPieces[index].y = y;
		boardGame[x][y] = &playerPieces[index];

		/* we did not toggle the turn in the last movePiece; now we do */
		toggle_next_turn();

		/* call HDMI to change type on screen */
		if (BoardDisplay_change_piece_type(type) != 0)
		{
			return INTERNAL_ERROR;
		}

		return OK;
	}
	else
	{
		return unathorized;
	}
}

/******************************************************************************
 * Compare the given password with the internal game password.
 * Should be called by REST module before most of its requests.
 ******************************************************************************/
enum ChessboardRestStatus validate_password(const char *pswd)
{
	xil_printf("Sent password: %s\nInternal password: %s\n", pswd, currentGameInfo.secret_code);
	if (strcmp(currentGameInfo.secret_code, pswd) != 0)
	{
		return unathorized;
	}
	return OK;
}

/******************************************************************************
 * Give information about the turn
 * (who's turn it is, move number, last move, game status).
 ******************************************************************************/
enum ChessboardRestStatus get_summary(TurnInfo *turnInfo)
{
	*turnInfo = currentTurnInfo;
	return OK;
}

/******************************************************************************
 * Give game's details, as configured by a /new_game request.
 ******************************************************************************/
enum ChessboardRestStatus get_game_info(GameInfo *gameInfo)
{
	*gameInfo = currentGameInfo;
	return OK;
}

/******************************************************************************
 * Give the position and aliveness of each piece of the chessboard.
 * Also gives the turn and move number.
 ******************************************************************************/
enum ChessboardRestStatus get_board(BoardPosition *boardPosition)
{
	boardPosition->turn_info = currentTurnInfo;
	int i = 0;
	for (i = 0; i < 16; i++)
	{
		if (player1Pieces[i].alive == true)
		{
			boardPosition->positions[i][0] = player1Pieces[i].x + 'a';
			boardPosition->positions[i][1] = player1Pieces[i].y + '1';
		}
		else
		{
			boardPosition->positions[i][0] = 'x';
			boardPosition->positions[i][1] = 'x';
		}
	}
	for (i = 0; i < 16; i++)
	{
		if (player2Pieces[i].alive == true)
		{
			boardPosition->positions[i+16][0] = player2Pieces[i].x + 'a';
			boardPosition->positions[i+16][1] = player2Pieces[i].y + '1';
		}
		else
		{
			boardPosition->positions[i+16][0] = 'x';
			boardPosition->positions[i+16][1] = 'x';
		}
	}
	return OK;

}

/******************************************************************************
 * Give a player its time informations (time remaining, overtime).
 ******************************************************************************/
enum ChessboardRestStatus get_time(int player,TimeInfo *timeInfo)
{
	// TODO
	return NOT_IMPLEMENTED;
}

/******************************************************************************
 * Set the board and pieces to their standard initial state, ready to play.
 ******************************************************************************/
static void ChessGameInitialisation()
{
	// we clear the board
	clearBoard();

	// we initialize the pieces of the first player
	player1Pieces[0] = PieceInitialisation(4,0,KING,player1);
	player1Pieces[1] = PieceInitialisation(3,0,QUEEN,player1);
	player1Pieces[2] = PieceInitialisation(2,0,BISHOP,player1);
	player1Pieces[3] = PieceInitialisation(5,0,BISHOP,player1);
	player1Pieces[4] = PieceInitialisation(0,0,ROOK,player1);
	player1Pieces[5] = PieceInitialisation(7,0,ROOK,player1);
	player1Pieces[6] = PieceInitialisation(1,0,KNIGHT,player1);
	player1Pieces[7] = PieceInitialisation(6,0,KNIGHT,player1);
	player1Pieces[8] = PieceInitialisation(0,1,PAWN,player1);
	player1Pieces[9] = PieceInitialisation(1,1,PAWN,player1);
	player1Pieces[10] = PieceInitialisation(2,1,PAWN,player1);
	player1Pieces[11] = PieceInitialisation(3,1,PAWN,player1);
	player1Pieces[12] = PieceInitialisation(4,1,PAWN,player1);
	player1Pieces[13] = PieceInitialisation(5,1,PAWN,player1);
	player1Pieces[14] = PieceInitialisation(6,1,PAWN,player1);
	player1Pieces[15] = PieceInitialisation(7,1,PAWN,player1);

	// place P1 pieces on the board
	setBoard(player1Pieces);

	// we initialize the pieces for the second player
	player2Pieces[0] = PieceInitialisation(4,7,KING,player2);
	player2Pieces[1] = PieceInitialisation(3,7,QUEEN,player2);
	player2Pieces[2] = PieceInitialisation(2,7,BISHOP,player2);
	player2Pieces[3] = PieceInitialisation(5,7,BISHOP,player2);
	player2Pieces[4] = PieceInitialisation(0,7,ROOK,player2);
	player2Pieces[5] = PieceInitialisation(7,7,ROOK,player2);
	player2Pieces[6] = PieceInitialisation(1,7,KNIGHT,player2);
	player2Pieces[7] = PieceInitialisation(6,7,KNIGHT,player2);
	player2Pieces[8] = PieceInitialisation(0,6,PAWN,player2);
	player2Pieces[9] = PieceInitialisation(1,6,PAWN,player2);
	player2Pieces[10] = PieceInitialisation(2,6,PAWN,player2);
	player2Pieces[11] = PieceInitialisation(3,6,PAWN,player2);
	player2Pieces[12] = PieceInitialisation(4,6,PAWN,player2);
	player2Pieces[13] = PieceInitialisation(5,6,PAWN,player2);
	player2Pieces[14] = PieceInitialisation(6,6,PAWN,player2);
	player2Pieces[15] = PieceInitialisation(7,6,PAWN,player2);

	// place P2 pieces on the board
	setBoard(player2Pieces);

	// initialize currentTurnInfo
	currentTurnInfo.game_status = NORMAL;
	currentTurnInfo.last_move_src[0] = 'x';
	currentTurnInfo.last_move_src[1] = 'x';
	currentTurnInfo.last_move_dst[0] = 'x';
	currentTurnInfo.last_move_dst[1] = 'x';
	currentTurnInfo.move_no = 1;
	currentTurnInfo.turn = player1;

	chessclock_init(&currentGameInfo);
}

/******************************************************************************
 * Set a piece to an initial state.
 ******************************************************************************/
static Piece PieceInitialisation(int x, int y,PieceType type, PlayerID playerID)
{
	Piece piece;
	piece.pieceType = type;
	piece.alive = true;
	piece.x = x;
	piece.y = y;
	piece.enPassant = false;
	piece.rock = 1;
	piece.playerID = playerID;
	return piece;
}

/******************************************************************************
 * Set the board so that each square that should be occupied by a piece has
 * the corresponding pointer. It does so only for one player's pieces.
 ******************************************************************************/
static void setBoard(Piece* playerPieces)
{
	int i = 0;
	// place P1 pieces on the board
	for (i = 0; i < 16; i++)
	{
		boardGame[playerPieces[i].x][playerPieces[i].y] = &playerPieces[i];
	}
}

/******************************************************************************
 * Set every square of the chessboard to NULL.
 ******************************************************************************/
static void clearBoard()
{
	int i = 0;
	for (i = 0; i < 8;i++)
	{
		int j = 0;
		for (j = 0; j < 8; j++)
		{
			boardGame[i][j] = 0; // TODO MAKE NULL
		}
	}
}

/******************************************************************************
 * Check if a piece's move is valid, depending on its type.
 ******************************************************************************/
enum moveResult execute_move(Piece *piece, int xs, int xd, int ys, int yd)
{
	enum moveResult result = ILLEGAL;
	switch (piece->pieceType)
	{
	case KING:
		result = move_king(xs, xd, ys, yd);
		break;

	case ROOK:
		result = move_rook(xs, xd, ys, yd);
		break;

	case BISHOP:
		result = move_bishop(xs, xd, ys, yd);
		break;

	case KNIGHT:
		result = move_knight(xs, xd, ys, yd);
		break;

	case QUEEN:
		result = move_queen(xs, xd, ys, yd);
		break;

	case PAWN:
		result = move_pawn(xs, xd, ys, yd);
		break;

	default:
		result = ILLEGAL; // unidentified piece type
	}
	return result;
}

/******************************************************************************
 * Check if a king's move is valid.
 * NOTE: Does not yet check for castling and check situations.
 ******************************************************************************/
static enum moveResult move_king(int xs, int xd, int ys, int yd)
{
	// Partially accepting castling
	// Castling will be specified by having the tablet request to move
	// the king.
	if(xs == E && boardGame[xs][ys]->rock != 0 &&
			(xd == G || xd == C)

	  ){
		return ILLEGAL;
	}
	// TODO: check for Castle/Roque/special move
	if (xs-xd<-1 || xs-xd>1 || ys-yd<-1 || ys-yd>1)
		return ILLEGAL; //moving too far
	// TODO: check for dangerous position
	return VALID;
}

/******************************************************************************
 * Check if a rook's move is valid.
 ******************************************************************************/
static enum moveResult move_rook(int xs, int xd, int ys, int yd)
{
	if (xs != xd)
	{
		if(ys!=yd)
			return ILLEGAL; // moving in both directions
		else // moving on the x axis
		{
			/* check squares between min+1 and max-1 for an obstructing piece */
			int i = (xs < xd ? xs + 1 : xd + 1); // min + 1
			for (; i < (xs > xd ? xs : xd); ++i) // i < max
				if(boardGame[i][ys] != 0)
					return ILLEGAL; // passing over a piece
		}
	}
	else // moving on the y axis
	{
		/* check squares between min+1 and max-1 for an obstructing piece */
		int i = (ys < yd ? ys + 1 : yd + 1); // min + 1
		for (; i < (ys > yd ? ys : yd); ++i) // i < max
			if(boardGame[xs][i] != 0)
				return ILLEGAL; // passing over a piece
	}
	return VALID;
}

/******************************************************************************
 * Check if a bishop's move is valid.
 ******************************************************************************/
static enum moveResult move_bishop(int xs, int xd, int ys, int yd)
{
	// not moving equally in both directions
	if (xs - xd  != ys - yd && xs - xd  != yd - ys) // diff X != diff Y && diff X != -diff Y
		return ILLEGAL;

	// moving along the y = x axis
	if ((xs < xd && ys < yd) || (xs > xd && ys > yd))
	{
		int i = (xs < xd ? xs : xd); // min X
		int j = (ys < yd ? ys : yd); // min Y
		int dist = (xs > xd ? xs - xd : xd - xs);
		int k = 1;
		for (; k < dist; ++k )
			if(boardGame[i+k][j+k] != 0)
				return ILLEGAL; // passing over a piece
	}

	// moving along the y = -x axis
	else
	{
		int i = (xs < xd ? xs : xd); // min X
		int j = (ys > yd ? ys : yd); // max Y
		int dist = (xs > xd ? xs - xd : xd - xs);
		int k = 1;
		for (; k < dist; ++k )
			if(boardGame[i+k][j-k] != 0)
				return ILLEGAL; // passing over a piece
	}
	return VALID;
}

/******************************************************************************
 * Check if a knight's move is valid.
 ******************************************************************************/
static enum moveResult move_knight(int xs, int xd, int ys, int yd)
{
	{
		int x = (xs > xd ? xs - xd : xd - xs); // abs(diff X)
		int y = (ys > yd ? ys - yd : yd - ys); // abs(diff Y)

		if (x + y != 3 || x == 3 || y == 3)// should move 3 squares but not all in the same direction
			return ILLEGAL;
	}
	return VALID;
}

/******************************************************************************
 * Check if a queen's move is valid.
 ******************************************************************************/
static enum moveResult move_queen(int xs, int xd, int ys, int yd)
{
	enum moveResult result = move_rook(xs, xd, ys, yd);
	if (result != VALID)
	{
		result = move_bishop(xs, xd, ys, yd);
		if (result != VALID)
			return result;
	}
	return VALID;
}

/******************************************************************************
 * Check if a pawn's move is valid.
 ******************************************************************************/
static enum moveResult move_pawn(int xs, int xd, int ys, int yd)
{
	Piece *piece = boardGame[xs][ys];
	if(	piece->playerID == player1 ?
		ys == 1 && yd == 3 && xs == xd && boardGame[xs][2] == 0 && boardGame[xs][3] == 0 :
		ys == 6 && yd == 4 && xs == xd && boardGame[xs][5] == 0 && boardGame[xs][4] == 0
		) // if first time jump and no one in the two front squares
		piece->enPassant = true;
	else if(yd - ys != (piece->playerID == player1 ? 1 : -1 )) // does not advance exactly 1 square
	{
		return ILLEGAL;
	}
	else
	{
		if ((xs > xd ? xs - xd : xd - xs) > 1) // abs(diff X) > 1
			return ILLEGAL; // moving too much in X
		if (xs == xd && boardGame[xd][yd] != NULL)
			return ILLEGAL; // capturing in front

		if((xs > xd ? xs - xd : xd - xs) == 1) // moving diagonnally / capturing
		{
			Piece *enPassant = boardGame[xd][ys]; // using ys instead of yd +/- 1
			if (currentGameInfo.en_passant == true && // En Passant is enabled
				enPassant != NULL && // capturing En Passant
				enPassant->enPassant == true && // can be captured En Passant
				(enPassant->playerID != piece->playerID)) // is not your own piece
			{
				return ENPASSANT;
			}
			else if (boardGame[xd][yd] == NULL) // not capturing on arrival
				return ILLEGAL; // not capturing
		}
	}

	return VALID;
}

/******************************************************************************
 * Check if a piece is up for promotion.
 * Depends on
 *     1. piece type (must be a pawn)
 *     2. player ID (sorry Michael, it DOES matter if you're black or white)
 *     3. piece position (obviously)
 ******************************************************************************/
bool can_promote(Piece *piece)
{
	/* 1. piece type (must be a pawn) */
	if (piece->pieceType != PAWN)
	{
		return false;
	}

	/* 2. player ID (sorry Michael, it DOES matter if you're black or white) */
	int promotion_row;
	if (piece->playerID == player1)
	{
		promotion_row = 7;
	}
	else
	{
		promotion_row = 0;
	}

	/* 3. piece position (obviously) */
	if (piece->y == promotion_row)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************
 * Find a piece type from its name. Can be king|queen|rook|bishop|knight|pawn.
 * Return -1 if type is invalid.
 ******************************************************************************/
static PieceType get_type(const char *type_name)
{
	if (strcmp(type_name, "king") == 0)
		return KING;
	else if (strcmp(type_name, "queen") == 0)
		return QUEEN;
	else if (strcmp(type_name, "rook") == 0)
		return ROOK;
	else if (strcmp(type_name, "bishop") == 0)
		return BISHOP;
	else if (strcmp(type_name, "knight") == 0)
		return KNIGHT;
	else if (strcmp(type_name, "pawn") == 0)
		return PAWN;
	else
		return -1;
}

/******************************************************************************
 * Searches for a captured piece to bring back in game and returns its index.
 * Return -1 if none is found.
 ******************************************************************************/
static int find_available_type(PieceType type, int player)
{
	Piece *playerPieces = (player == 1 ? player1Pieces : player2Pieces);
	int i;

	/* find the first captured piece fitting the type */
	for (i = 0; i < 16; i++)
		if (playerPieces[i].pieceType == type && !playerPieces[i].alive)
			return i;

	return -1;	/* None found */
}

/******************************************************************************
 * When a turn ends, we have to add an increment to the clock, set the turn
 * to the next player, and increment the move number.
 ******************************************************************************/
static void toggle_next_turn()
{
	// At the end of a turn, have the clock add an increment
	chessclock_add_increment(&currentGameInfo,currentTurnInfo.turn); // Increment current player's time before changing whose turn it is.
	currentTurnInfo.turn = (currentTurnInfo.move_no%2 + 1);
	currentTurnInfo.move_no++;
}

/******************************************************************************
 *
 ******************************************************************************/
static void clear_enPassant(int player)
{
	Piece *playerPieces;
	if (player == 1)
	{
		playerPieces = player1Pieces;
	}
	else
	{
		playerPieces = player2Pieces;
	}
	int i = 0;
	for (i = 0; i < 8; i++)
		playerPieces[i+8].enPassant = false;
}
