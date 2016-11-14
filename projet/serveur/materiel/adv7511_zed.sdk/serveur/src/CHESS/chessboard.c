#include "chessboard.h"
#include <string.h>
#include "xil_io.h"
#include "debug.h"
#include "BoardDisplay.h"
//player1 pieces




enum moveResult{
	VALID,
	ILLEGAL,
	ENPASSANT,
	CASTLING
};
enum moveResult execute_move(Piece *piece, int xs, int xd, int ys, int yd); // defined at the end of the file but used before then.

static Piece* boardGame[8][8];
static Piece player1Pieces[16];
static Piece player2Pieces[16];

static Piece PieceInitialisation(int x, int y,PieceType type, PlayerID playerID)
{
	Piece piece;
	piece.pieceType = type;
	piece.alive = true;
	piece.x = x;
	piece.y = y;
	piece.enPassant = false;
	piece.rock = 0;
	piece.playerID = playerID;
	return piece;
}

//game bools
static bool gameStarted = false;
static bool player1Turn = true;

//structs

static GameInfo currentGameInfo;
static TurnInfo currentTurnInfo;

enum ChessboardRestStatus validate_password(const char *pswd)
{
	if (strcmp(currentGameInfo.secret_code, pswd) != 0)
	{
		return unathorized;
	}
	return OK;
}

void setBoard(Piece* playerPieces)
{
	int i = 0;
	// place P1 pieces on the board
	for (i = 0; i < 16; i++)
	{
		boardGame[playerPieces[i].x][playerPieces[i].y] = &playerPieces[i];
	}
}

void clearBoard()
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

static void ChessGameInitialisation()
{
	// we clear the board
	clearBoard();
	
	// we initialize the pieces of the first players
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
	player1Turn = true;
	currentTurnInfo.game_status = NORMAL;
	currentTurnInfo.last_move[0] = 'x';
	currentTurnInfo.last_move[1] = 'x';
	currentTurnInfo.move_no = 1;
	currentTurnInfo.turn = player1;
}


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
		BoardDisplay_new_board(&currentGameInfo);
		return OK;	
	}

}


enum ChessboardRestStatus movePiece(int player, const char *src, const char *dst, MoveInfo* moveInfo)
{
	struct Move mv; // infos for BoardDisplay/HDMI
	mv.enPassant = false;

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
	if (xs<0 || xs>7 || ys<0 || ys>7 || xd<0 || xd>7 || yd<0 || yd>7)
		return deplacementIllegal; // out of the board

	if (boardGame[xs][ys] == 0)
		return deplacementIllegal; // no piece here

	if (xs == xd && ys == yd)
		return deplacementIllegal; // not moving

	Piece *piece = boardGame[xs][ys];

	if (piece->playerID != player)
		return deplacementIllegal; // opponent piece

	switch(execute_move(piece, xs, xd, ys, yd))
	{
	case ILLEGAL:
		return deplacementIllegal;

	case CASTLING:
		return NOT_IMPLEMENTED;

	case ENPASSANT:
		boardGame[xd][ys]->alive = false; // special capture using ys instead of yd +/- 1
		boardGame[xd][ys] = 0; // special cleaning
		moveInfo->piece_eliminated[0] = xd + 'a';
		moveInfo->piece_eliminated[1] = ys + '1';
		mv.enPassant = true;
		break;

	case VALID:
		break;

	default:
		return NOT_IMPLEMENTED;
	}


	if (boardGame[xd][yd] != 0)
	{
		if (boardGame[xd][yd]->playerID == player)
			return deplacementIllegal; // capturing allied piece
		boardGame[xd][yd]->alive = false; // capture enemy piece
		moveInfo->piece_eliminated[0] = xd + 'a';
		moveInfo->piece_eliminated[1] = yd + '1';
	}
	else
	{
		moveInfo->piece_eliminated[0] = 'x';
		moveInfo->piece_eliminated[1] = 'x';
	}
	// we dont check for promotion right now

	//setboard position

	boardGame[xd][yd] = piece; // move the piece
	boardGame[xs][ys] = 0; // clear the source space
	boardGame[xd][yd]->x = xd;
	boardGame[xd][yd]->y = yd;

	// increment turn, change player turn, time stuff

	currentTurnInfo.turn = (currentTurnInfo.move_no%2 + 1);
	currentTurnInfo.move_no++;
	currentTurnInfo.last_move[0] = xd + 'a';
	currentTurnInfo.last_move[1] = yd + '1';
	currentTurnInfo.game_status = NORMAL; // FOR NOW
	moveInfo->game_status = NORMAL;
	moveInfo->promotion = false;

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

	//check for stalemate, checkmate,
	// not implemented

	return OK;
}

enum ChessboardRestStatus promote_piece(int player, const char *new_type)
{
	return NOT_IMPLEMENTED;
}

//function for second release
enum ChessboardRestStatus get_time(int player,TimeInfo *timeInfo)
{
	return NOT_IMPLEMENTED;
}


enum ChessboardRestStatus get_summary(TurnInfo *turnInfo)
{
	*turnInfo = currentTurnInfo;
	return OK;
}

enum ChessboardRestStatus get_board(BoardPosition *boardPosition)
{
	boardPosition->turn = currentTurnInfo.turn;
	boardPosition->move_no = currentTurnInfo.move_no;
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

enum ChessboardRestStatus set_board(BoardPosition *boardPosition)
{
// cleasr the board
	ChessGameInitialisation();
	clearBoard();
	
	// we set player1 pieces 
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
	
	// we set player 2 pieces
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


	// we set the board
	setBoard(player1Pieces);
	setBoard(player2Pieces);
	currentTurnInfo.turn = boardPosition->turn;
	currentTurnInfo.move_no = boardPosition->move_no;
	if (BoardDisplay_draw_pieces_custom(player1Pieces,player2Pieces) != 0)
	{
		xil_printf("Error in HDMI restSetBoard");
	}
	return OK;
	
}

enum ChessboardRestStatus get_game_info(GameInfo *gameInfo)
{
	*gameInfo = currentGameInfo;
	return OK;
}

enum ChessboardRestStatus start_game()
{
	if (gameStarted)
	{
		return gameInProgress;
	}
	gameStarted = true;
	BoardDisplay_start_game();
	return OK;
}

/******************************************************************************
 * Ends a game: Return to initial state
 * NOTE: new_game creates a game but does not start it so for now, I put the
 * boardDisplay_welcome_screen call in the error case and in the non-error case
******************************************************************************/
enum ChessboardRestStatus end_game()
{
	FBEGIN;
	if (!gameStarted)
	{
		return unathorized;
	}
	gameStarted = false;
	BoardDisplay_welcome_screen();
	return OK;
}


enum ChessboardRestStatus move_king(int xs, int xd, int ys, int yd)
{
	// Partially accepting castling
	// Castling will be specified by having the tablet request to move
	// the king.
	if(xs == E && (xd == G || xd == C)){
		return ILLEGAL;
	}
	// TODO: check for Castle/Roque/special move
	if (xs-xd<-1 || xs-xd>1 || ys-yd<-1 || ys-yd>1)
		return ILLEGAL; //moving too far
	// TODO: check for dangerous position
	return VALID;
}

// uses static boardGame
enum ChessboardRestStatus move_rook(int xs, int xd, int ys, int yd)
{
	if (xs != xd)
	{
		if(ys!=yd)
			return ILLEGAL; // moving in both directions
		else
		{ // moving on the x
			int i = (xs < xd ? xs + 1 : xd + 1); // min + 1
			for (; i < (xs > xd ? xs : xd); ++i) // i < max
				if(boardGame[i][ys] != 0)
					return ILLEGAL; // passing over a piece
		}
	}
	else
	{ // moving on the y
		int i = (ys < yd ? ys + 1 : yd + 1); // min + 1
		for (; i < (ys > yd ? ys : yd); ++i) // i < max
			if(boardGame[xs][i] != 0)
				return ILLEGAL; // passing over a piece
	}
	return VALID;
}

// uses static boardGame
enum ChessboardRestStatus move_bishop(int xs, int xd, int ys, int yd)
{
	//if ( (xs > xd ? xs - xd : xd - xs) != (ys > yd ? ys - yd : yd - ys) ) // abs(diff X) != abs(diff Y)
	if (xs - xd  != ys - yd && xs - xd  != yd - ys) // diff X != diff Y && diff X != -diff Y
		return ILLEGAL; // not moving equally in both directions

	// Francis was here j'ai rajouter : || (xs > xd && ys > yd) a : 	if ((xs < xd && ys < yd) || (xs > xd && ys > yd))
	if ((xs < xd && ys < yd) || (xs > xd && ys > yd))
	{ // meme signe
		int i = (xs < xd ? xs : xd); // min X
		int j = (ys < yd ? ys : yd); // min Y
		int dist = (xs > xd ? xs - xd : xd - xs);
		int k = 1;
		for (; k < dist; ++k )
			if(boardGame[i+k][j+k] != 0)
				return ILLEGAL; // passing over a piece
	}
	else
	{ // signe different
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

enum ChessboardRestStatus move_knight(int xs, int xd, int ys, int yd)
{
	{
		int x = (xs > xd ? xs - xd : xd - xs); // abs(diff X)
		int y = (ys > yd ? ys - yd : yd - ys); // abs(diff Y)

		if (x + y != 3 || x == 3 || y == 3)// should move 3 squares but not all in the same direction
			return ILLEGAL;
	}
	return VALID;
}
enum ChessboardRestStatus move_queen(int xs, int xd, int ys, int yd)
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

// uses static boardGame, player1Pieces and player2Pieces and may change their values.
enum moveResult move_pawn(int xs, int xd, int ys, int yd)
{
	Piece *piece = boardGame[xs][ys];
	if(	piece->playerID == player1 ?
		ys == 1 && yd == 3 && xs == xd && boardGame[xs][2] == 0 :
		ys == 6 && yd == 4 && xs == xd && boardGame[xs][5] == 0
		) // if first time jump
		piece->enPassant = true; // TODO: clean this flag on the next turn
	else if(yd - ys != (piece->playerID == player1 ? 1 : -1 )) // does not advance exactly 1 square
	{
		return ILLEGAL;
	}
	else
	{
		if ((xs > xd ? xs - xd : xd - xs) > 1) // abs(diff X) > 1
			return ILLEGAL; // moving too much in X
		if (xs == xd && boardGame[xd][yd] != 0)
			return ILLEGAL; // capturing in front

		if((xs > xd ? xs - xd : xd - xs) == 1) // moving diagonnally / capturing
		{
			Piece *enPassant = boardGame[xd][ys]; // using ys instead of yd +/- 1
			if (currentGameInfo.en_passant != 0 && // En Passant is enabled
				enPassant != 0 && // capturing En Passant
				enPassant->enPassant == true && // can be captured En Passant
				(enPassant->playerID == player1 ? yd ==  5: yd == 2)) // is not your own piece
			{
				return ENPASSANT;
			}
			else if (boardGame[xd][yd] == 0) // not capturing on arrival
				return ILLEGAL; // not capturing
		}
	}
	//promote stuff if promote
	return VALID;
}

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

