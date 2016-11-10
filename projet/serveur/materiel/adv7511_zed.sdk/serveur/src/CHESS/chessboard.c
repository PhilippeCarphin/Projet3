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
	int enPassant; // TODO: could become a bool. Should be set after the permitted jump and unset the turn after.
	PlayerID playerID;
}Piece;

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
	piece.enPassant = 0;
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
	player1Pieces[0] = PieceInitialisation(4,0,king,player1);
	player1Pieces[1] = PieceInitialisation(3,0,queen,player1);
	player1Pieces[2] = PieceInitialisation(2,0,bishop,player1);
	player1Pieces[3] = PieceInitialisation(5,0,bishop,player1);
	player1Pieces[4] = PieceInitialisation(0,0,rook,player1);
	player1Pieces[5] = PieceInitialisation(7,0,rook,player1);
	player1Pieces[6] = PieceInitialisation(1,0,knight,player1);
	player1Pieces[7] = PieceInitialisation(6,0,knight,player1);
	player1Pieces[8] = PieceInitialisation(0,1,pawn,player1);
	player1Pieces[9] = PieceInitialisation(1,1,pawn,player1);
	player1Pieces[10] = PieceInitialisation(2,1,pawn,player1);
	player1Pieces[11] = PieceInitialisation(3,1,pawn,player1);
	player1Pieces[12] = PieceInitialisation(4,1,pawn,player1);
	player1Pieces[13] = PieceInitialisation(5,1,pawn,player1);
	player1Pieces[14] = PieceInitialisation(6,1,pawn,player1);
	player1Pieces[15] = PieceInitialisation(7,1,pawn,player1);
	
	// place P1 pieces on the board
	setBoard(player1Pieces);

	// we initialize the pieces for the second player
	player2Pieces[0] = PieceInitialisation(4,7,king,player2);
	player2Pieces[1] = PieceInitialisation(3,7,queen,player2);
	player2Pieces[2] = PieceInitialisation(2,7,bishop,player2);
	player2Pieces[3] = PieceInitialisation(5,7,bishop,player2);
	player2Pieces[4] = PieceInitialisation(0,7,rook,player2);
	player2Pieces[5] = PieceInitialisation(7,7,rook,player2);
	player2Pieces[6] = PieceInitialisation(1,7,knight,player2);
	player2Pieces[7] = PieceInitialisation(6,7,knight,player2);
	player2Pieces[8] = PieceInitialisation(0,6,pawn,player2);
	player2Pieces[9] = PieceInitialisation(1,6,pawn,player2);
	player2Pieces[10] = PieceInitialisation(2,6,pawn,player2);
	player2Pieces[11] = PieceInitialisation(3,6,pawn,player2);
	player2Pieces[12] = PieceInitialisation(4,6,pawn,player2);
	player2Pieces[13] = PieceInitialisation(5,6,pawn,player2);
	player2Pieces[14] = PieceInitialisation(6,6,pawn,player2);
	player2Pieces[15] = PieceInitialisation(7,6,pawn,player2);
	
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
		// draw_hdmi_init_board();
		return OK;	
	}
}

enum ChessboardRestStatus move_piece(int player, const char *src, const char *dst, MoveInfo* moveInfo)
{

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

	switch (piece->pieceType)
	{
	case king:
		// TODO: check for Castle/Roque/special move
		if (xs-xd<-1 || xs-xd>1 || ys-yd<-1 || ys-yd>1)
			return deplacementIllegal; //moving too far
		// TODO: check for dangerous position
		break;

	case rook:
		if (xs != xd)
		{
			if(ys!=yd)
				return deplacementIllegal; // moving in both directions
			else
			{ // moving on the x
				int i = (xs < xd ? xs + 1 : xd + 1); // min + 1
				for (; i < (xs > xd ? xs : xd); ++i) // i < max
					if(boardGame[i][ys] != 0)
						return deplacementIllegal; // passing over a piece
			}
		}
		else
		{ // moving on the y
			int i = (ys < yd ? ys + 1 : yd + 1); // min + 1
			for (; i < (ys > yd ? ys : yd); ++i) // i < max
				if(boardGame[xs][i] != 0)
					return deplacementIllegal; // passing over a piece
		}
		break;

	case bishop:
		//if ( (xs > xd ? xs - xd : xd - xs) != (ys > yd ? ys - yd : yd - ys) ) // abs(diff X) != abs(diff Y)
		if (xs - xd  != ys - yd && xs - xd  != yd - ys) // diff X != diff Y && diff X != -diff Y
			return deplacementIllegal; // not moving equally in both directions

		if ((xs < xd && ys < yd))
		{ // meme signe
			int i = (xs < xd ? xs : xd); // min X
			int j = (ys < yd ? ys : yd); // min Y
			int dist = (xs > xd ? xs - xd : xd - xs);
			int k = 1;
			for (; k < dist; ++k )
				if(boardGame[i+k][j+k] != 0)
					return deplacementIllegal; // passing over a piece
		}
		else
		{ // signe different
			int i = (xs < xd ? xs : xd); // min X
			int j = (ys > yd ? ys : yd); // max Y
			int dist = (xs > xd ? xs - xd : xd - xs);
			int k = 1;
			for (; k < dist; ++k )
				if(boardGame[i+k][j-k] != 0)
					return deplacementIllegal; // passing over a piece
		}
		break;

	case knight:
		{
			int x = (xs > xd ? xs - xd : xd - xs); // abs(diff X)
			int y = (ys > yd ? ys - yd : yd - ys); // abs(diff Y)

			if (x + y != 3 || x == 3 || y == 3)// should move 3 squares but not all in the same direction
				return deplacementIllegal;
		}
		break;

	case queen:
		if (xs != xd || ys != yd) // not moving like a Rook
		{
			if (xs - xd  != ys - yd && xs - xd  != yd - ys) // not moving like a Bishop
				return deplacementIllegal; // not moving equally in both directions
			 // moving like a Bishop
			if ((xs < xd && ys < yd))
			{ // meme signe

				int i = (xs < xd ? xs : xd); // min X
				int j = (ys < yd ? ys : yd); // min Y
				int dist = (xs > xd ? xs - xd : xd - xs);
				int k = 1;
				for (; k < dist; ++k )
					if(boardGame[i+k][j+k] != 0)
						return deplacementIllegal; // passing over a piece
			}
			else
			{ // signe different
				int i = (xs < xd ? xs : xd); // min X
				int j = (ys > yd ? ys : yd); // max Y
				int dist = (xs > xd ? xs - xd : xd - xs);
				int k = 1;
				for (; k < dist; ++k )
					if(boardGame[i+k][j-k] != 0)
						return deplacementIllegal; // passing over a piece
			}

		}
		else
		{
			// moving like a Rook
			if (xs != xd)
			{ // moving on the x
				int i = (xs < xd ? xs + 1 : xd + 1); // min + 1
				for (; i < (xs > xd ? xs : xd); ++i) // i < max
					if(boardGame[i][ys] != 0)
						return deplacementIllegal; // passing over a piece

			}
			else
			{ // moving on the y
				int i = (ys < yd ? ys + 1 : yd + 1); // min + 1
				for (; i < (ys > yd ? ys : yd); ++i) // i < max
					if(boardGame[xs][i] != 0)
						return deplacementIllegal; // passing over a piece
			}

		}
		break;
	case pawn:
		if(	piece->playerID == player1 ?
			ys == 1 && yd == 3 && xs == xd && boardGame[xs][2] == 0 :
			ys == 6 && yd == 4 && xs == xd && boardGame[xs][5] == 0
			) // if first time jump
			piece->enPassant = true; // TODO: clean this flag on the next turn
		else if(yd - ys != (piece->playerID == player1 ? 1 : -1 )) // does not advance exactly 1 square
		{
			return deplacementIllegal;
		}
		else
		{
			if ((xs > xd ? xs - xd : xd - xs) > 1) // abs(diff Y) > 1
				return deplacementIllegal; // moving too much in Y
			if (xs == xd && boardGame[xd][yd] != 0)
				return deplacementIllegal; // capturing in front

			if((xs > xd ? xs - xd : xd - xs) == 1) // moving diagonnally / capturing
			{
				Piece *enPassant = boardGame[xd][ys]; // using ys instead of yd +/- 1
				if (enPassant != 0 && // capturing En Passant
					enPassant->enPassant == true && // can be captured En Passant
					enPassant->playerID == player1 ? yd ==  5: yd == 2) // is not your own piece
				{
					enPassant->alive = false; // special capture
					boardGame[xd][ys] = 0; // special cleaning
				}
				else if (boardGame[xd][yd] == 0) // not capturing on arrival
					return deplacementIllegal; // not capturing
			}
		}
		//promote stuff if promote
		break;

	default:
		return NOT_IMPLEMENTED; // unidentified piece type
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

	boardGame[xd][yd] = piece; // move the piece
	boardGame[xs][ys] = 0; // clear the source space
	boardGame[xd][yd]->x = xd;
	boardGame[xd][yd]->y = yd;

	// increment turn, change player turn, time stuff
	//call HDMI draw functions
	//setboard position
	//check for path, checkmate,
	currentTurnInfo.turn = (currentTurnInfo.move_no%2 + 1);
	currentTurnInfo.move_no++;
	currentTurnInfo.game_status = NORMAL; // FOR NOW
	moveInfo->promotion = false;
	// not implemented
	moveInfo->game_status = NORMAL;
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
	return OK;
}

enum ChessboardRestStatus end_game()
{
	if (!gameStarted)
	{
		return NOT_IMPLEMENTED;
	}
	gameStarted = false;
	return OK;
}

