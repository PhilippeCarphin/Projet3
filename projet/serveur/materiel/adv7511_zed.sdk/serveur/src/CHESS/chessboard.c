#include "chessboard.h"
//player1 pieces

typedef enum {pawn, knight, rook, bishop, queen, king }PieceType;
typedef enum { false = 0 , true = 1 } bool;
typedef struct Piece
{
	PieceType pieceType;
	bool alive;
	int x;
	int y;
	int rock;
	int enPassant;
	bool player1;
}Piece;

static Piece* boardGame[8][8];
static Piece player1Pieces[16];
static Piece player2Pieces[16];

static Piece PieceInitialisation(int x, int y,PieceType type, bool isPlayer1)
{
	Piece piece;
	piece.pieceType = type;
	piece.alive = true;
	piece.x = x;
	piece.y = y;
	piece.enPassant = 0;
	piece.rock = 0;
	piece.player1 = isPlayer1;
	return piece;
}

//game bools
static bool gameStarted = false;
static bool player1Turn = true;

//structs

static GameInfo currentGameInfo;
static TurnInfo currentTurnInfo;


static void ChessGameInitialisation()
{
	// we initialize the pieces of the first players
	player1Pieces[0] = PieceInitialisation(3,0,king,true);
	player1Pieces[1] = PieceInitialisation(4,0,queen,true);
	player1Pieces[2] = PieceInitialisation(2,0,bishop,true);
	player1Pieces[3] = PieceInitialisation(5,0,bishop,true);
	player1Pieces[4] = PieceInitialisation(0,0,rook,true);
	player1Pieces[5] = PieceInitialisation(7,0,rook,true);
	player1Pieces[6] = PieceInitialisation(1,0,knight,true);
	player1Pieces[7] = PieceInitialisation(6,0,knight,true);
	player1Pieces[8] = PieceInitialisation(0,1,pawn,true);
	player1Pieces[9] = PieceInitialisation(1,1,pawn,true);
	player1Pieces[10] = PieceInitialisation(2,1,pawn,true);
	player1Pieces[11] = PieceInitialisation(3,1,pawn,true);
	player1Pieces[12] = PieceInitialisation(4,1,pawn,true);
	player1Pieces[13] = PieceInitialisation(5,1,pawn,true);
	player1Pieces[14] = PieceInitialisation(6,1,pawn,true);
	player1Pieces[15] = PieceInitialisation(7,1,pawn,true);
	
	// we initialize the pieces for the second player
	player2Pieces[0] = PieceInitialisation(3,7,king,false);
	player2Pieces[1] = PieceInitialisation(4,7,queen,false);
	player2Pieces[2] = PieceInitialisation(2,7,bishop,false);
	player2Pieces[3] = PieceInitialisation(5,7,bishop,false);
	player2Pieces[4] = PieceInitialisation(0,7,rook,false);
	player2Pieces[5] = PieceInitialisation(7,7,rook,false);
	player2Pieces[6] = PieceInitialisation(1,7,knight,false);
	player2Pieces[7] = PieceInitialisation(6,7,knight,false);
	player2Pieces[8] = PieceInitialisation(0,6,pawn,false);
	player2Pieces[9] = PieceInitialisation(1,6,pawn,false);
	player2Pieces[10] = PieceInitialisation(2,6,pawn,false);
	player2Pieces[11] = PieceInitialisation(3,6,pawn,false);
	player2Pieces[12] = PieceInitialisation(4,6,pawn,false);
	player2Pieces[13] = PieceInitialisation(5,6,pawn,false);
	player2Pieces[14] = PieceInitialisation(6,6,pawn,false);
	player2Pieces[15] = PieceInitialisation(7,6,pawn,false);

	
	int i =0;
	for (i = 0; i < 16; i++)
	{
		boardGame[player1Pieces[i].x][player1Pieces[i].y] = &player1Pieces[i];
	}
	
	for (i = 0; i < 16; i++)
	{
		boardGame[player2Pieces[i].x][player2Pieces[i].y] = &player2Pieces[i];
	}
	
	gameStarted = true;
	player1Turn = true;
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
		// draw_hdmi_init_board();
		return OK;	
	}
}

enum ChessboardRestStatus move_piece(int player, const char *src, const char *dst, MoveInfo* moveInfo)
{
	return NOT_IMPLEMENTED;
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
			boardPosition->positions[i][1] = player1Pieces[i].y + 1;
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
			boardPosition->positions[i+16][1] = player2Pieces[i].y + 1;
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
	return NOT_IMPLEMENTED;
}

enum ChessboardRestStatus get_game_info(GameInfo *gameInfo)
{
	return NOT_IMPLEMENTED;
}

enum ChessboardRestStatus start_game()
{
	return NOT_IMPLEMENTED;
}

enum ChessboardRestStatus end_game()
{
	return NOT_IMPLEMENTED;
}
