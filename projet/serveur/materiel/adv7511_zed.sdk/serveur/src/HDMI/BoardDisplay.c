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

enum Colors { 	BACKGROUND = 0xFF000000,
				DARK_SQUARE_COLOR = 0xFFEEEED2,
				LIGHT_SQUARE_COLOR = 0xFF769656,
				YELLOW = 0xFFFFFF00,
				MARGIN_COLOR = 0xFF955C3E
};


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
 * Declarations of functions
******************************************************************************/

int draw_piece(PieceType type, PieceColor color, File file, Rank rank);
int color_square(File file, Rank rank, u32 color);
int clear_square(File file, Rank rank);
int BoardDisplay_init();
int set_chess_board_params();
int draw_chess_board();
int BoardDisplay_move_piece(struct Move *move);
u32 file_to_pixel(int file);
u32 rank_to_pixel(int rank);

/******************************************************************************
 * Used by BoardDisplay_move_piece, reset by
******************************************************************************/
static int first_move = 1;

// New stuff
int draw_char(u32 screen_top, u32 screen_left, char c);
int draw_string(u32 screen_top, u32 screen_left, char *str);
// int update_times(struct Player_times);

/******************************************************************************
 * Set parameters in chessboard data.
******************************************************************************/
int set_chess_board_params()
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
int load_bitmap_files()
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
 * Set buffer addresses from stack-declared buffers in main().
******************************************************************************/
int BoardDisplay_set_image_buffers(u8 *chars_dat, u8 *pieces_dat)
{
	chars_data = chars_dat;
	pieces_data = pieces_dat;
	return 0;
}

/******************************************************************************
 * Get the offset associated with the piece type.
******************************************************************************/
u32 getPieceOffset(PieceType p)
{
	switch(p)
	{
	case KING:
		return 8;
	case PAWN:
		return 19;
	case QUEEN:
		return 1;
	case BISHOP:
		return 6;
	case KNIGHT:
		return 8;
	case ROOK:
		return 13;
	default:
		return 0;
	}
}


/******************************************************************************
 * Read bitmap data into memory, initialize offsets
******************************************************************************/
int BoardDisplay_init()
{
	int err = 0;
	set_screen_dimensions(1280,1024);
	set_chess_board_params();
	set_background_color(0x00000000); 
	load_bitmap_files();
	draw_chess_board();
	return err;
}


/******************************************************************************
 * Draw a char, note that draw_partial_bitamp checks the screen dimensions so
 * if we draw a char outside the screen, it's not going to do anything except
 * maybe waste processor time.
******************************************************************************/
int draw_char(u32 screen_top, u32 screen_left, char c)
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
int draw_string(u32 screen_top, u32 screen_left, char *str)
{
	u32 cursor_top = screen_top;
	u32 cursor_left = screen_left;
	int err;
	char c;
	while( (c = *str++) != 0)
	{
		if( c == '\n'){
			cursor_top += line_skip;
			cursor_left = screen_left;
		}
		else if( c == ' '){
			cursor_left += char_width;
		}
		else if ( 32 < c && c <= '~' )
		{
			if( cursor_left + char_width + 2 >= screen.w){
				cursor_left = screen_left;
				cursor_top += line_skip;
			}
			if( (err =draw_char(cursor_top, cursor_left, c)) != 0){
				WHERE DBG_PRINT("Could not draw char %c\n");
				return err;
			} else {
				cursor_left += char_width;
			}
		} else {
			DBG_PRINT("%s(): Unknown char\n", __FUNCTION__);
		}

	}
	return 0;
}

/******************************************************************************
 * Something like this to draw the information on the screen
******************************************************************************/
int draw_information(struct GameInfo *gi)
{
	//int some_top = 0, some_left = 0;
	//draw_string(some_top, some_left, gi->player1);
	//draw_string(some_top, some_left, gi->player2);
	// ...

	return 0;
}

/******************************************************************************
 * Something like this would be called regularly to make the times count down.
******************************************************************************/
int update_times(struct PlayerTimes *pt)
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

/******************************************************************************
 * Draw a piece on the chessboard.  With piece positions in the image specified
 * by the enum PieceType and PieceColor.  Position in the board given by the
 * file and rank params.
******************************************************************************/
int draw_piece(PieceType type, PieceColor color, File file, Rank rank)
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
int color_square(File file, Rank rank, u32 color)
{
	u32 screen_top = rank_to_pixel(rank);
	u32 screen_left = file_to_pixel(file);
	return draw_square(screen_top, screen_left, 
						bd.square_size, bd.square_size, color);
}

/******************************************************************************
 * Set the color of a square to the correct color
******************************************************************************/
int clear_square(File file, Rank rank)
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
int draw_empty_board()
{
	int file = A;
	int rank = R1;
	int err;

	// Dessiner la marge (un carré plus gros en arriere du board).
	draw_square(bd.top - bd.margin,bd.left - bd.margin,
			8*bd.square_size + 2*bd.margin, 8*bd.square_size + 2*bd.margin,
			MARGIN_COLOR);

	// Dessiner les carrés verts et blancs
	for( file = A; file <= H; file++)
		for( rank = R1; rank <= R8; rank++)
			if((err = clear_square(file,rank)) != 0) return err;

	// Dessiner le rectangle pour la zone de notation
	draw_square(bd.notation_top, bd.notation_left, bd.notation_width, bd.notation_height, 0x00000000);
	draw_coordinates();
	return 0;
}

/******************************************************************************
 * Draw coordinates
******************************************************************************/
int draw_coordinates()
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
int draw_pieces()
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
 * Draw pieces in custom positions
******************************************************************************/
int draw_pieces_custom(Piece* player1, Piece* player2)
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

/******************************************************************************
 * Draws the chessboard with pieces in their initial positions
******************************************************************************/
int test_move_piece();
 int draw_chess_board()
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
 char getPieceChar(PieceType t)
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
 	default:
 		return ' ';
 	}
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
 int draw_move_notation(struct Move *mv)
 {
 	int move_number = (mv->turn_number-1)/2;
 	u32 cursor_top = bd.notation_top + move_number * line_skip;
 	u32 cursor_left = bd.notation_left;

 	/*
 	 * For a white move, draw the move number on the complete left and a dot.
 	 * Otherwise, move the cursor 9 positions to the right to draw the black
 	 * move.
 	 */
 	if( mv->c == WHITE)
 	{
 		draw_char(cursor_top,cursor_left,'0' + move_number + 1);
 		cursor_left += char_width;
 		draw_char(cursor_top,cursor_left, '.');
 		cursor_left += 2*char_width;
 	}
 	else
 	{
 		cursor_left += 9 * char_width;
 	}

 	if( mv->castling)
 	{
 		if(mv->d_file == G)
 			draw_string(cursor_top, cursor_left, "O-O");
 		else
 			draw_string(cursor_top, cursor_left, "O-O-O");

 		return 0;
 	}

 	/*
 	 * We draw the char identifying the piece (K,N,B,R) and nothing for pawns,
 	 * except if the move is a capturing move, in which case, we identify the
 	 * moved pawn by it's origin file (dx4e meaning the pawn on 'd' takes on
 	 * e4)
 	 */
 	if(mv->t != PAWN)
 	{
 		draw_char(cursor_top, cursor_left, getPieceChar(mv->t));
 		cursor_left += char_width;
 	} else if (mv->capture){
 		draw_char(cursor_top, cursor_left, 'a' + mv->o_file);
 		cursor_left += char_width;
 	}

 	/*
 	 * If the move is a capture, we draw an 'x' between the char identifying
 	 * the piece and the destination square.
 	 */
	if( mv->capture )
	{
		draw_char(cursor_top, cursor_left, 'x'); cursor_left += char_width;
	}

	/*
	 * And we end by drawing the destination square
	 */
 	char f = 'a' + mv->d_file;
 	char r = '1' + mv->d_rank;
 	draw_char(cursor_top, cursor_left, f); cursor_left += char_width;
 	draw_char(cursor_top, cursor_left, r); cursor_left += char_width;

 	return 0;
 }

/******************************************************************************
 * Used to remove the yellow squares of the move parameter.  This is used on 
 * the previous move that BoardDisplay_move_piece remembers.
******************************************************************************/
 int un_yellow(struct Move *mv)
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
int do_move(struct Move *mv)
{
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
		if((err = color_square(mv->o_file, mv->o_rank, YELLOW)) != 0) return err;
		if((err = color_square(mv->d_file, mv->d_rank, YELLOW)) != 0) return err;
		if((err = draw_piece(mv->t, mv->c, mv->d_file, mv->d_rank)) != 0) return err;
	}
	return 0;
}

/******************************************************************************
 * This function executes a move and writes to the notation rectangle.
******************************************************************************/
int BoardDisplay_move_piece(struct Move *move)
{
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

	cf_hdmi_send_buffer();

	return 0;
}

/******************************************************************************
 *
******************************************************************************/
u32 file_to_pixel(int file)
{
	return bd.left + file * bd.square_size;
}

/******************************************************************************
 *
******************************************************************************/
u32 rank_to_pixel(int rank)
{
	return bd.top + (7 - rank) * bd.square_size;
}

int test_move_piece()
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
