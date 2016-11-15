#include "bitmap.h"

#include "xil_io.h"
#include "DrawHDMI.h"
#include "BoardDisplay.h"
#define DEBUG
#include "debug.h"
#include "cf_hdmi.h"

#define MIN(x, y)	(x < y ? x : y)
#define MAX(x, y) 	(x > y ? x : y)

/*****************************************************************************
 * Struct containing info on the board, 
 * - (top,left) decide where to place the by specifying the position of it's top
 *   left corner in the screen
 * - margin specifies the width of the margin around the board.
 * - square_size determines the width and height of the squares in pixels
 * - the notation zone is a black rectangle where game notation will be displayed
 *   and we specify it's top,left corner as well as the width and height.
******************************************************************************/
struct BoardData{
	u32 top;
	u32 left;
	u32 square_size;
	u32 notation_top;
	u32 notation_left;
	u32 notation_width;
	u32 notation_height;
	u32 margin;
};
static struct BoardData bd;

enum Colors { 	BACKGROUND = 0xFF666666,
				GAME_BACKGROUND = 0xFF888888,
				DARK_SQUARE_COLOR = 0xFFEEEED2,
				LIGHT_SQUARE_COLOR = 0xFF769656,
				YELLOW = 0xFFFFFF00,
				MARGIN_COLOR = 0xFF955C3E
};

static char readme[] =
"Welcome to Vintage Chess.\n\n"
"To start a game\n\n"
"\t1. Start Vintage Chess and enter the IP address \n"
"\t   that is shown below.If you are too old to understand\n"
"\t   what is an IP address, call technical services\n"
"\t   at 514-978-1336\n\n"
"\t2. Press Start\n\n"
"\t3. Enter information in the fields\n\n"
"\t4. Press Create Game\n";

extern struct Screen screen;
/******************************************************************************
 * Stuff pertaining to bitmaps of characters and pieces
******************************************************************************/
static BMP chars;
static u8 *chars_data;
static int char_width = 15;
static int line_skip = 22; // line_skip should be more than the height of a letter;

static BMP pieces;
static u8 *pieces_data;
static int piece_height = 204 - WHITE;

#define v_offset 10;
#define h_offset 10;


/******************************************************************************
 * Used by BoardDisplay_move_piece, reset by
******************************************************************************/
static int first_move = 1;

/******************************************************************************
 * Declarations of functions
******************************************************************************/
static int draw_information(GameInfo *gi);
static int draw_turn(PieceColor c);
// static int update_times(struct PlayerTimes *pt);
static int draw_piece(PieceType type, PieceColor color, File file, Rank rank);
static int color_square(File file, Rank rank, u32 color);
static int clear_square(File file, Rank rank);
static int draw_empty_board();
static int draw_coordinates();
static int draw_pieces();
static int draw_chess_board();
static int draw_move_number(u32 top, u32 left, int number);
static int draw_move_notation(struct Move *mv);
static int un_yellow(struct Move *mv);
static int do_move(struct Move *mv);
static u32 file_to_pixel(int file);
static u32 rank_to_pixel(int rank);
static int set_chess_board_params();
static int load_bitmap_files();
static int draw_char(u32 screen_top, u32 screen_left, char c);
static int draw_string(u32 screen_top, u32 screen_left, char *str);
static u32 getPieceOffset(PieceType p);
// static int test_move_piece();



/******************************************************************************
 * Set buffer addresses from stack-declared buffers in main().
******************************************************************************/
int BoardDisplay_set_image_buffers(u8 *chars_dat, u8 *pieces_dat)
{
	chars_data = chars_dat;
	pieces_data = pieces_dat;
	return 0;
}

/******************************************************************************
 * Read bitmap data into memory, initialize offsets
******************************************************************************/
int BoardDisplay_init()
{
	int err = 0;
	set_screen_dimensions(1280,1024);
	set_chess_board_params();
	load_bitmap_files();
	return err;
}

/******************************************************************************
 * Display the welcome screen.
******************************************************************************/
extern u32 ip_last_byte;
int BoardDisplay_welcome_screen()
{
	set_background_color(BACKGROUND);
	draw_string(100,100,readme);

	char ip_string_buffer[100] = {0};
	sprintf(ip_string_buffer, "Server is running on ip : 132.207.89.%lu", ip_last_byte);
	draw_string(500,100,ip_string_buffer);

	cf_hdmi_send_buffer();
	return 0;
}

/******************************************************************************
 * Displays a fresh board with pieces setup and game information.  Since the 
 * game might not be started yet, BoardDisplay_draw_turn() is not called.
******************************************************************************/
int BoardDisplay_new_board(GameInfo *gi)
{
	int err;
	set_background_color(GAME_BACKGROUND);
	if( (err = draw_chess_board()) != 0) return err;
	if( (err = draw_information(gi)) != 0) return err;
	cf_hdmi_send_buffer();
	return 0;
}

/******************************************************************************
 * Add additionnal information to the screen after the game has started.
 * For now, it will only draw "White to move".
******************************************************************************/
int BoardDisplay_start_game()
{
	if(draw_turn(WHITE) != 0) return -1;
	cf_hdmi_send_buffer();
	return 0;
}

/******************************************************************************
 * This function executes a move and writes to the notation rectangle.
******************************************************************************/
int BoardDisplay_move_piece(struct Move *move)
{
	FBEGIN;
	int err;

	move->castling = (move->t == KING 
						&& move->o_file == E
						&& (move->d_file == G || move->d_file == C));
	/*
	 * During the preceding call, we yellowed the origin square,
	 * and we drew the moved piece on a yellowed square.  So we start 
	 * by restoring this to normal by clearing both squares and redrawing
	 * the moved piece on the cleared square
	 */
	static struct Move last;
	if( ! first_move )
	{
		if((err = un_yellow(&last)) != 0) return err;
	}

	/*
	 * Yellow the origin square and the destination square and draw the moved
	 * piece on it's destination square.
	 */
	if((err = do_move(move)) != 0) return err;

	if((err = draw_move_notation(move)) != 0) return err;
	
	/*
	 * Remember the last move for the next one.
	 */
	first_move = 0;
	last = *move;

	draw_turn((move->c == WHITE ? BLACK : WHITE));
	cf_hdmi_send_buffer();
	FEND;
	return 0;
}

/******************************************************************************
 * Draw pieces in custom positions
******************************************************************************/
int BoardDisplay_draw_pieces_custom(Piece* player1, Piece* player2)
{
	FBEGIN;
	draw_empty_board();
	int err;
	int i = 0;
	for (i = 0; i < 16; ++i)
	{
		if (player1[i].x != 'x')
		{
			if((err = draw_piece(player1[i].pieceType, WHITE,player1[i].x,player1[i].y)) != 0) return err;
		}
		if (player2[i].x != 'x')
		{
			if((err = draw_piece(player2[i].pieceType, BLACK,player2[i].x,player2[i].y)) != 0) return err;
		}

	}
	cf_hdmi_send_buffer();
	FEND;
	return 0;
}

#if 0 // TODO
int setup_board()
{
	set_background_color(BACKGROUND);
	draw_empty_board();
	BoardDisplay_draw_pieces_custom(...);// TODO
}
#endif

/******************************************************************************
 * Something like this to draw the information on the screen
******************************************************************************/
static int draw_information(GameInfo *gi)
{
	u32 top = bd.top + 8*bd.square_size + 3 * bd.margin;
	u32 left = bd.left;
	u32 cursor_top = top;
	u32 cursor_left = left;

	// Draw player names
	draw_string(cursor_top, cursor_left, "White : \nBlack : ");
	cursor_left += 8*char_width;
	draw_string(cursor_top, cursor_left, gi->player_1);
	cursor_top += line_skip;
	draw_string(cursor_top, cursor_left, gi->player_2);

	// Draw event info
	cursor_top = top;
	cursor_left = left + 20 * char_width;
	draw_string(cursor_top, cursor_left, "   Event :\nLocation :\n   Round :"); cursor_left += 10*char_width;
	draw_string(cursor_top, cursor_left, "Livrable 1!"); cursor_top += line_skip;
	draw_string(cursor_top, cursor_left, gi->location); cursor_top += line_skip;
	draw_string(cursor_top, cursor_left, gi->round);
	//draw_string(some_top, some_left, gi->player2);
	// ...

	char ip_buff_2[100] = {0};
	sprintf(ip_buff_2,"IP is 132.207.89.%lu", ip_last_byte);

	u32 width = char_width * strlen(ip_buff_2) - 3;
	cursor_left = 1150 - width;
	cursor_top = 1000;//1080 - line_skip - 30;
	draw_square(cursor_top, cursor_left, width, line_skip, 0);
	draw_string(cursor_top, cursor_left, ip_buff_2);

	return 0;
}

/******************************************************************************
 * Draws "White to move" or "Black to move" under the chessboard to indicate
 * whose turn it is to play.
******************************************************************************/
static int draw_turn(PieceColor c)
{
	int err;
	u32 top = bd.top + 8*bd.square_size + bd.margin + 10;
	u32 left = bd.left + D * bd.square_size;
	draw_square(top,left, 13 * char_width, line_skip, 0);
	if( c == WHITE)
	{
		if( (err = draw_string(top, left, "White to move")) != 0) return err;
	}
	else
	{
		if( (err = draw_string(top, left, "Black to move")) != 0) return err;
	}
	return 0;
}

#if 0
/******************************************************************************
 * Something like this would be called regularly to make the times count down.
******************************************************************************/
static int update_times(struct PlayerTimes *pt)
{
	int err;
	int left = 0, top = 0;
	int height = 20, width = 500;
	if((err = draw_square(top, left, width, height, BACKGROUND)) != 0) return err;
	if((err = draw_string(top, left, pt->player1)) != 0) return err;

	if((err = draw_square(top, left, width, height, BACKGROUND)) != 0) return err;
	if((err = draw_string(top, left, pt->player2)) != 0) return err;

	return 0;
}
#endif

/******************************************************************************
 * Draw a piece on the chessboard.  With piece positions in the image specified
 * by the enum PieceType and PieceColor.  Position in the board given by the
 * file and rank params.
******************************************************************************/
static int draw_piece(PieceType type, PieceColor color, File file, Rank rank)
{
	u32 bmp_top = color;
	u32 bmp_left = type;

	u32	bmp_bottom = bmp_top + piece_height;
	u32 bmp_right = MIN(bmp_left + 90,800);

	u32 screen_top = rank_to_pixel(rank) + v_offset;
	u32 screen_left = file_to_pixel(file) + getPieceOffset(type);

	return draw_partial_bitmap( screen_top , screen_left,
								bmp_top,    bmp_left,
								bmp_bottom, bmp_right,
								&pieces, pieces_data);
}

/******************************************************************************
 * Color a square in the screen
******************************************************************************/
static int color_square(File file, Rank rank, u32 color)
{
	u32 screen_top = rank_to_pixel(rank);
	u32 screen_left = file_to_pixel(file);
	return draw_square(screen_top, screen_left, 
						bd.square_size, bd.square_size, color);
}

/******************************************************************************
 * Set the color of a square to the correct color
******************************************************************************/
static int clear_square(File file, Rank rank)
{
	u32 color;
	if( (rank + file) % 2 == 0 )
		color = LIGHT_SQUARE_COLOR;
	else
		color = DARK_SQUARE_COLOR;

	return color_square(file, rank, color);
}

/******************************************************************************
 * Draw an empty chessboard with margin, squares and coordinates
******************************************************************************/
static int draw_empty_board()
{
	int file = A;
	int rank = R1;
	int err;

	// Dessiner la marge (un carr� plus gros en arriere du board).
	draw_square(bd.top - bd.margin,bd.left - bd.margin,
			8*bd.square_size + 2*bd.margin, 8*bd.square_size + 2*bd.margin,
			MARGIN_COLOR);

	// Dessiner les carr�s verts et blancs
	for( file = A; file <= H; file++)
		for( rank = R1; rank <= R8; rank++)
			if((err = clear_square(file,rank)) != 0) return err;

	draw_coordinates();

	// Dessiner le rectangle pour la zone de notation
	draw_square(bd.notation_top, bd.notation_left, bd.notation_width, bd.notation_height, 0x00000000);
	draw_square(bd.notation_top, bd.notation_left, bd.notation_width, line_skip, 0x00303030);
	draw_string(bd.notation_top, bd.notation_left, "GAME NOTATION");

	return 0;
}

/******************************************************************************
 * Draw coordinates
******************************************************************************/
static int draw_coordinates()
{
	int err;
	char c;
	int offset = bd.square_size / 2 - 10;
	for(c = 'A'; c <= 'H'; ++c)
	{
		if((err = draw_char(bd.top + 8*bd.square_size, bd.left + offset + (c-'A')*bd.square_size, c)) != 0) return err;
		if((err = draw_char(bd.top - 24 , bd.left + offset + (c-'A')*bd.square_size, c)) != 0) return err;
	}

	for(c = '8'; c >= '1'; --c)
	{
		if((err = draw_char(bd.top + offset + ('8'-c) * bd.square_size, bd.left - 17, c)) != 0) return err;
		if((err = draw_char(bd.top + offset + ('8'-c) * bd.square_size, bd.left + 8*bd.square_size +3, c)) != 0) return err;
	}
	return 0;
}

/******************************************************************************
 * Draw pieces in their initial positions
******************************************************************************/
static int draw_pieces()
{
	int err;
	File file;
	// Dessiner les pions.
	for( file = A; file <= H; file++)
	{
		if((err = draw_piece(PAWN, WHITE, file, R2)) != 0) return err;
		if((err = draw_piece(PAWN, BLACK, file, R7)) != 0) return err;
	}

	// Dessiner les pieces blanches
	if((err = draw_piece(ROOK,   WHITE, A, R1)) != 0) return err;
	if((err = draw_piece(ROOK,   WHITE, H, R1)) != 0) return err;
	if((err = draw_piece(KNIGHT, WHITE, B, R1)) != 0) return err;
	if((err = draw_piece(KNIGHT, WHITE, G, R1)) != 0) return err;
	if((err = draw_piece(BISHOP, WHITE, C, R1)) != 0) return err;
	if((err = draw_piece(BISHOP, WHITE, F, R1)) != 0) return err;
	if((err = draw_piece(KING,   WHITE, E, R1)) != 0) return err;
	if((err = draw_piece(QUEEN,  WHITE, D, R1)) != 0) return err;

	// Dessiner les pieces noires
	if((err = draw_piece(ROOK,   BLACK, A, R8)) != 0) return err;
	if((err = draw_piece(ROOK,   BLACK, H, R8)) != 0) return err;
	if((err = draw_piece(KNIGHT, BLACK, B, R8)) != 0) return err;
	if((err = draw_piece(KNIGHT, BLACK, G, R8)) != 0) return err;
	if((err = draw_piece(BISHOP, BLACK, C, R8)) != 0) return err;
	if((err = draw_piece(BISHOP, BLACK, F, R8)) != 0) return err;
	if((err = draw_piece(KING,   BLACK, E, R8)) != 0) return err;
	if((err = draw_piece(QUEEN,  BLACK, D, R8)) != 0) return err;

	return 0;
}

/******************************************************************************
 * Draws the chessboard with pieces in their initial positions
******************************************************************************/
static int draw_chess_board()
{
	int err;

	if( (err = set_background_color(0x00666666)) != 0) return err;
	if( (err = draw_empty_board()) != 0) return err;
	if( (err = draw_pieces()) != 0) return err;

	first_move = 1;
	cf_hdmi_send_buffer();
	// test_move_piece();
	return 0;
}

 /******************************************************************************
  * Gets the char associated with a piece
 ******************************************************************************/
static char getPieceChar(PieceType t)
 {
 	switch(t){
 	case KNIGHT:
 		return 'N';
 	case KING:
 		return 'K';
 	case BISHOP:
 		return 'B';
 	case ROOK:
 		return 'R';
 	case QUEEN:
 		return 'Q';
 	default:
 		return 'W';
 	}
 }

 /******************************************************************************
  * Draw the move number (2 digits max) and a '.'
 ******************************************************************************/
static int draw_move_number(u32 top, u32 left, int number)
 {
	if( number <= 0)
	{
		WHERE DBG_PRINT("Invalid move_number\n");
		return -1;
	}
	u32 cursor_left = left;
	u32 cursor_top = top;
	int tens = number / 10;
	int units = number % 10;

	if(tens != 0)
	{
		draw_char(cursor_top, cursor_left, '0' + tens);
	}

	cursor_left += char_width;
	draw_char(cursor_top, cursor_left, '0' + units);
	cursor_left += char_width;
	draw_char(cursor_top, cursor_left, '.');

	return 0;
 }

 /******************************************************************************
  * Draws the move notation of a move.
  * Standard chess notation works by specifying the least amount of information.
  * So we say the piece that moved, and the destination of that piece.  
  * For pawn moves that are not captures, there is no ambiguity in simply
  * specifying the destination square.
  * NOTE: One thing that is not taken into account here is the case when there
  * is more than one piece of a given type that can go to a square, for example,
  * if there are white rooks on a4 and h4, with nothing between them, in that 
  * case the move Re4 is ambiguous and we need to write Rae4 to specify that
  * the rook on a4 moved to e4 and the one on h4.
  * One way to deal with this is to adop the slighly less standard notation of
  * always specifying the origin square.
  * The other alternative is to have the chessboard module prepare the string
  * representing the move.  In any case, writing chess notation to the screen
  * was easy for Phil to do and that's the only reason it is there.
 ******************************************************************************/
 static int draw_move_notation(struct Move *mv)
 {
	FBEGIN;
 	int move_number = (mv->turn_number + 1)/2;
 	char console_out[30] = {0};
 	char buff[30] = {0};
 	u32 cursor_top = bd.notation_top + (move_number + 1 ) * line_skip;
 	u32 cursor_left = bd.notation_left;

 	/*
 	 * For a white move, draw the move number on the complete left and a dot.
 	 * Otherwise, move the cursor 9 positions to the right to draw the black
 	 * move.
 	 */
 	WHEREL;
 	if( mv->c == WHITE)
 	{
 		draw_move_number(cursor_top, cursor_left, move_number);
 		sprintf(buff, "%d. ", move_number);
 		cursor_left += 5*char_width;
 	}
 	else
 	{
 		cursor_left += 10 * char_width;
 		sprintf(buff, "%d. ... ", move_number);
 	}
 	strcat(console_out, buff);

 	if( mv->castling)
 	{
 		char *castling_str;
 		if(mv->d_file == G)
 		{
 			castling_str = "O-O";
 		}
 		else
 		{
 			castling_str = "O-O-O";
 		}

		draw_string(cursor_top, cursor_left, castling_str);
		strcat(console_out, castling_str);
		xil_printf(console_out);
		FEND;
 		return 0;
 	}

 	WHEREL;
 	/*
 	 * We draw the char identifying the piece (K,N,B,R) and nothing for pawns,
 	 * except if the move is a capturing move, in which case, we identify the
 	 * moved pawn by it's origin file (dx4e meaning the pawn on 'd' takes on
 	 * e4)
 	 */
 	if(mv->t != PAWN)
 	{
 		char type_c = getPieceChar(mv->t);
 		draw_char(cursor_top, cursor_left, type_c);
 		sprintf(buff, "%c", type_c);
 		cursor_left += char_width;
 	} else if (mv->capture){
 		draw_char(cursor_top, cursor_left, 'a' + mv->o_file);
 		cursor_left += char_width;
 		sprintf(buff, "%c", 'a' + mv->o_file);
 	} else {
 		buff[0] = '\0';
 	}
 	strcat(console_out, buff);

 	/*
 	 * If the move is a capture, we draw an 'x' between the char identifying
 	 * the piece and the destination square.
 	 */
	if( mv->capture )
	{
		draw_char(cursor_top, cursor_left, 'x'); cursor_left += char_width;
		strcat(console_out, "x");
	}

	/*
	 * And we end by drawing the destination square
	 */
 	char f = 'a' + mv->d_file;
 	char r = '1' + mv->d_rank;
 	draw_char(cursor_top, cursor_left, f); cursor_left += char_width;
 	draw_char(cursor_top, cursor_left, r); cursor_left += char_width;
 	sprintf(buff, "%c%c",f,r);
 	strcat(console_out, buff);

 	xil_printf("Move made (FIDE notation) %s\n", console_out);

 	FEND;
 	return 0;
 }

/******************************************************************************
 * Used to remove the yellow squares of the move parameter.  This is used on 
 * the previous move that BoardDisplay_move_piece remembers.
******************************************************************************/
static int un_yellow(struct Move *mv)
{
	int err;
	if( mv->castling)
	{
		// Redraw the king on a non-yellow square
		if((err = clear_square(mv->d_file,mv-> d_rank)) != 0) return err;
		if((err = draw_piece(KING, mv->c, mv->d_file,mv->d_rank)) != 0) return err;
		// Redraw the rook on a non-yellow square
		// File rook_file = (mv->d_file == C ? D : F);
		// if((err = clear_square(rook_file, mv->d_rank)) != 0) return err;
		// if((err = draw_piece(ROOK, mv->c, rook_file ,mv->d_rank)) != 0) return err;
	}
	else
	{
		// Clear last origin square,
		if((err = clear_square(mv->o_file, mv->o_rank)) != 0) return err;
		// Redraw the piece on a non-yellow square.
		if((err = clear_square(mv->d_file, mv->d_rank)) != 0) return err;
		if((err = draw_piece(mv->t, mv->c, mv->d_file, mv->d_rank)) != 0) return err;
	}
	return 0;
}

/******************************************************************************
 * Used to move implicated in the move.  This checks if the move is a castling
 * move, or an en-passant capture and does things appropiately.  Otherwise, it
 * will simply clear the destination and origin squares with yellow, then draw
 * the piece on the destination square.
 * NOTE: the previous move is remembered and the un_yellow() function is used
 * to restore the right color under the pieces that were moved when the next
 * move is played.
******************************************************************************/
static int do_move(struct Move *mv)
{
	FBEGIN;
	int err;
	if( mv->castling )
	{
		// clear origin
		if((err = clear_square(mv->o_file, mv->o_rank)) != 0) return err;;
		if((err = color_square(mv->d_file, mv->d_rank, YELLOW)) != 0) return err;;
		// yellow king square
		// draw king
		if((err = draw_piece(KING, mv->c, mv->d_file, mv->d_rank)) != 0) return err;;

		if( mv->d_file == G )
		{
			if((err = clear_square(H, mv->o_rank)) != 0) return err;
			if((err = draw_piece(ROOK, mv->c, F, mv->o_rank)) != 0) return err;
		}
		else
		{
			if((err = clear_square(A, mv->o_rank)) != 0) return err;
			if((err = draw_piece(ROOK, mv->c, D, mv->o_rank)) != 0) return err;
		}
	}
	else
	{
#if 0
		if(mv->enPassant)
		{
			/*
			 * we have to clear a different square then the destination square, the square is on 
			 * the same file as the destination square but it is one rank below if white made the
			 * en-passant capture, and one rank below if black made the en-passant capture.
			 */
			Rank r = (mv->c == WHITE ? mv->d_rank-1 : mv->d_rank+1);
			if((err = clear_square(mv->d_file, r)) != 0) return err;
		}
#endif

		if((err = color_square(mv->o_file, mv->o_rank, YELLOW)) != 0) return err;
		if((err = color_square(mv->d_file, mv->d_rank, YELLOW)) != 0) return err;
		if((err = draw_piece(mv->t, mv->c, mv->d_file, mv->d_rank)) != 0) return err;
	}
	FEND;
	return 0;
}

/******************************************************************************
 * Converts a File (A, ..., H) to a horizontal pixel coordinate (j) on the 
 * screen.
******************************************************************************/
static u32 file_to_pixel(int file)
{
	return bd.left + file * bd.square_size;
}

/******************************************************************************
 * Converts a Rank (R1, ..., R8) to a vertical pixel coordinate (i) on the 
 * screen.
******************************************************************************/
static u32 rank_to_pixel(int rank)
{
	return bd.top + (7 - rank) * bd.square_size;
}


/******************************************************************************
 * Set parameters in chessboard data.
******************************************************************************/
static int set_chess_board_params()
{
	bd.top = 50;
	bd.left = 50;
	bd.square_size = 100;
	bd.margin = 30;
	bd.notation_left = bd.left + 8*bd.square_size + bd.margin + 10;
	bd.notation_top = bd.top - bd.margin;
	bd.notation_width = 15 * char_width;
	bd.notation_height = 40 * line_skip;
	return 0;
}

/******************************************************************************
 * Load the images and their data into our memory buffers
******************************************************************************/
static int load_bitmap_files()
{
	int err = 0;
	if( (err = read_bitmap_file("LTT.bmp", &chars, chars_data, CHARS_DATA_SIZE)) != 0){
		WHERE DBG_PRINT("Unsuccessful load of Letters.bmp\n");
		return err;
	}
	if( (err = read_bitmap_file("CP2.bmp", &pieces, pieces_data, PIECE_DATA_SIZE)) != 0){
		WHERE DBG_PRINT("Unsuccessful load of ChessPieces.bmp\n");
		return err;
	}
	return 0;
}

/******************************************************************************
 * Draw a char, note that draw_partial_bitamp checks the screen dimensions so
 * if we draw a char outside the screen, it's not going to do anything except
 * maybe waste processor time.
******************************************************************************/
static int draw_char(u32 screen_top, u32 screen_left, char c)
{
	int offset = c - '!';

	u32 bmp_top = 0;
	u32 bmp_left = offset * char_width;

	u32 bmp_bottom = chars.Height;
	u32 bmp_right = bmp_left + char_width;

	return draw_partial_bitmap( screen_top, screen_left,
								bmp_top,    bmp_left,
								bmp_bottom, bmp_right,
								&chars, chars_data);
}

/******************************************************************************
 * Draw a string on the screen including the possiblity for having strings with
 * linefeeds in them.
******************************************************************************/
static int draw_string(u32 screen_top, u32 screen_left, char *str)
{
	u32 cursor_top = screen_top;
	u32 cursor_left = screen_left;
	int err;
	char c;
	while( (c = *str++) != 0)
	{
		if ( 32 < c && c <= '~' )
		{
#if 0
			if( cursor_left + char_width + 2 >= screen.w){
				cursor_left = screen_left;
				cursor_top += line_skip;
			}
#endif
			if( (err =draw_char(cursor_top, cursor_left, c)) != 0){
				WHERE DBG_PRINT("Could not draw char %c\n");
				return err;
			} else {
				cursor_left += char_width;
			}
		} else if( c == '\n'){
			cursor_top += line_skip;
			cursor_left = screen_left;
		}
		else if( c == ' '){
			cursor_left += char_width;
		}
		else if (c == '\t'){
			cursor_left += 4*char_width;
		}
		else {
			DBG_PRINT("%s(): Unknown char\n", __FUNCTION__);
		}

	}
	return 0;
}


/******************************************************************************
 * Get the offset associated with the piece type.
******************************************************************************/
static u32 getPieceOffset(PieceType p)
{
	switch(p)
	{
	case KING:
		return 14;
	case PAWN:
		return 25;
	case QUEEN:
		return 6;
	case BISHOP:
		return 10;
	case KNIGHT:
		return 14;
	case ROOK:
		return 19;
	default:
		return 0;
	}
}

#if 0
static int test_move_piece()
{
	static int t = 1;
	static struct Move mv;
	switch(t){
	case 1:
		mv.c = WHITE;
		mv.t = PAWN;
		mv.o_file = E;
		mv.o_rank = R2;
		mv.d_file = E;
		mv.d_rank = R4;
		mv.turn_number = 1;
		mv.capture = 0;
		break;
	case 2:
		mv.c = BLACK;
		mv.t = PAWN;
		mv.o_file = E;
		mv.o_rank = R7;
		mv.d_file = E;
		mv.d_rank = R5;
		break;
	case 3:
		mv.c = WHITE;
		mv.t = KNIGHT;
		mv.o_file = G;
		mv.o_rank = R1;
		mv.d_file = F;
		mv.d_rank = R3;
		break;
	case 4:
		mv.c = BLACK;
		mv.t = KNIGHT;
		mv.o_file = B;
		mv.o_rank = R8;
		mv.d_file = C;
		mv.d_rank = R6;
		break;
	case 5:
		mv.c = WHITE;
		mv.t = PAWN;
		mv.o_file = D;
		mv.o_rank = R2;
		mv.d_file = D;
		mv.d_rank = R4;
		mv.capture = 0;
		break;
	case 6:
		mv.c = BLACK;
		mv.t = PAWN;
		mv.o_file = E;
		mv.o_rank = R5;
		mv.d_file = D;
		mv.d_rank = R4;
		mv.capture = 1;
		break;
	case 7:
		mv.c = WHITE;
		mv.t = BISHOP;
		mv.o_file = F;
		mv.o_rank = R1;
		mv.d_file = B;
		mv.d_rank = R5;
		mv.capture = 0;
		break;
	case 8:
		mv.c = BLACK;
		mv.t = PAWN;
		mv.o_file = A;
		mv.o_rank = R7;
		mv.d_file = A;
		mv.d_rank = R6;
		mv.capture = 0;
		break;
	case 9:
		mv.c = WHITE;
		mv.t = BISHOP;
		mv.o_file = B;
		mv.o_rank = R5;
		mv.d_file = C;
		mv.d_rank = R6;
		mv.capture = 1;
		break;
	case 10:
		mv.c = BLACK;
		mv.t = PAWN;
		mv.o_file = D;
		mv.o_rank = R7;
		mv.d_file = C;
		mv.d_rank = R6;
		mv.capture = 1;
		break;
	case 11:
		mv.c = WHITE;
		mv.t = KING;
		mv.o_file = E;
		mv.o_rank = R1;
		mv.d_file = G;
		mv.d_rank = R1;
		mv.capture = 0;
		break;
	case 12:
		mv.c = BLACK;
		mv.t = KNIGHT;
		mv.o_file = G;
		mv.o_rank = R8;
		mv.d_file = F;
		mv.d_rank = R6;
		break;
	default:
		draw_string(bd.top + 8 * bd.square_size + 100, bd.left, "Marie-Eve est super cute!");
	}
	BoardDisplay_move_piece(&mv);
	mv.turn_number = ++t;

	return 0;
}
#endif
