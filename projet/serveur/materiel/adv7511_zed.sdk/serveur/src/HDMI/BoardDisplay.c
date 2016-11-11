#include "bitmap.h"

#include "xil_io.h"
#include "DrawHDMI.h"
#include "BoardDisplay.h"
#include "debug.h"
#include "cf_hdmi.h"

#define MIN(x, y)	(x < y ? x : y)
#define MAX(x, y) 	(x > y ? x : y)

/*****************************************************************************
 *
******************************************************************************/
struct BoardData{
	int top;
	int left;
	int square_size;
	int notation_top;
	int notation_left;
	int notation_width;
	int notation_height;
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
static u32 offsets[6];

#define v_offset 10;
#define h_offset 10;
/******************************************************************************
 * Declarations of functions
******************************************************************************/

int draw_piece(PieceType type, PieceColor color, File file, Rank rank);
int color_square(File file, Rank rank, u32 color);
int clear_square(File file, Rank rank);
int BoardDisplay_init();
int set_chess_board_params(int top, int left, int square_size, u32 margin);
int draw_chess_board();
int move_piece(struct Move *move);
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
int set_chess_board_params(int top, int left, int square_size, u32 margin)
{
	bd.top = 50;
	bd.left = 50;
	bd.square_size = square_size;
	bd.margin = margin;
	bd.notation_left = bd.left + 8*bd.square_size + bd.margin + 10;
	bd.notation_top = bd.top - bd.margin;
	bd.notation_width = 15 * char_width;
	bd.notation_height = 40 * line_skip;
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
 * Read bitmap data into memory, initialize offsets
******************************************************************************/
int BoardDisplay_init()
{
	int err;
	set_screen_dimensions(1280,1024);
	set_chess_board_params(200,50,90,25);
	set_background_color(0x00000000); // Set the entire screen to blue.

	if( (err = read_bitmap_file("LTT.bmp", &chars, chars_data, CHARS_DATA_SIZE)) != 0){
		WHERE DBG_PRINT("Unsuccessful load of Letters.bmp\n");
		return err;
	}
	if( (err = read_bitmap_file("CP2.bmp", &pieces, pieces_data, PIECE_DATA_SIZE)) != 0){
		WHERE DBG_PRINT("Unsuccessful load of ChessPieces.bmp\n");
		return err;
	}

	offsets[PAWN] = 19; //19
	offsets[ROOK] = 13; //13
	offsets[QUEEN] = 1;
	offsets[KNIGHT] = 8;
	offsets[KING] = 8;
	offsets[BISHOP] = 6;


	draw_chess_board();
	return 0;
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
	u32 screen_left = file_to_pixel(file) + offsets[type];

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

	// Dessiner la marge (un carr� plus gros en arriere du board).
	draw_square(bd.top - bd.margin,bd.left - bd.margin,
			8*bd.square_size + 2*bd.margin, 8*bd.square_size + 2*bd.margin,
			MARGIN_COLOR);

	// Dessiner les carr�s verts et blancs
	for( file = A; file <= H; file++)
		for( rank = R1; rank <= R8; rank++)
			if((err = clear_square(file,rank)) != 0) return err;

	// Dessiner le rectangle pour la zone de notation
	draw_square(bd.notation_top, bd.notation_left, bd.notation_width, bd.notation_height, 0x00000000);

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
 * Draws the chessboard with pieces in their initial positions
******************************************************************************/
int test_move_piece();
 int draw_chess_board()
{
	int err;

	if( (err = set_background_color(0x00666666)) != 0) return err;
	if( (err = draw_empty_board()) != 0) return err;
	if( (err = draw_coordinates()) != 0) return err;
	if( (err = draw_pieces()) != 0) return err;

	first_move = 1;

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
  * Draws the move notation of a move
 ******************************************************************************/
 int draw_move_notation(struct Move *mv)
 {
 	int move_number = (mv->turn_number-1)/2;
 	u32 cursor_top = bd.notation_top + move_number * line_skip;
 	u32 cursor_left = bd.notation_left;

 	/*
 	 * For a white move, draw the move number on the complete left and a dot.
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


 	/*
 	 * When
 	 */
 	if(mv->t != PAWN)
 	{
 		draw_char(cursor_top, cursor_left, getPieceChar(mv->t));
 		cursor_left += char_width;
 	} else if (mv->capture){
 		draw_char(cursor_top, cursor_left, 'a' + mv->o_file);
 		cursor_left += char_width;
 	}

	if( mv->capture )
	{
		draw_char(cursor_top, cursor_left, 'x'); cursor_left += char_width;
	}



 	char f = 'a' + mv->d_file;
 	char r = '1' + mv->d_rank;

 	draw_char(cursor_top, cursor_left, f); cursor_left += char_width;
 	draw_char(cursor_top, cursor_left, r); cursor_left += char_width;

 	return 0;
 }

/******************************************************************************
 * Put the preceding functions to use to do what Clement (Badass) Lanteigne
 * suggested with the coloring of squares
 * For maximum error reporting, we should do 
 * if( function() ) return -1;
******************************************************************************/
int BoardDisplay_move_piece(struct Move *move)
{
	int err;
	/*
	 * During the preceding call, we yellowed the origin square,
	 * and we drew the moved piece on a yellowed square.  So we start 
	 * by restoring this to normal by clearing both squares and redrawing
	 * the moved piece on the cleared square
	 */
	static struct Move last;
	if( ! first_move )
	{
		if((err = clear_square(last.o_file, last.o_rank)) != 0) return err;
		if((err = clear_square(last.d_file, last.d_rank)) != 0) return err;
		if((err = draw_piece(last.t, last.c, last.d_file, last.d_rank)) != 0) return err;
	}

	/*
	 * Yellow the origin square and the destination square and draw the moved
	 * piece on it's destination square.
	 */
	if((err = color_square(move->o_file, move->o_rank, YELLOW)) != 0) return err;
	if((err = color_square(move->d_file, move->d_rank, YELLOW)) != 0) return err;
	if((err = draw_piece(move->t, move->c, move->d_file, move->d_rank)) != 0) return err;
	draw_move_notation(move);
	
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
	default:
		draw_string(bd.top + 8 * bd.square_size + 100, bd.left, "Marie-Eve est super cute!");
	}
	BoardDisplay_move_piece(&mv);
	mv.turn_number = ++t;

	return 0;
}
