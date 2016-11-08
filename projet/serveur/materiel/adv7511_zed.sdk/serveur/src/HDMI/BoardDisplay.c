#include "bitmap.h"
#include "xil_types.h"

#include "xil_io.h"
#include "DrawHDMI.h"
#include "BoardDisplay.h"
#include "debug.h"

#define MIN(x, y)	(x < y ? x : y)
#define MAX(x, y) 	(x > y ? x : y)

/*****************************************************************************
 *
******************************************************************************/
struct BoardData{
	int top;
	int left;
	int square_size;
	u32 margin;
};
static struct BoardData bd;

enum Colors { 	BACKGROUND = 0x00000000,
				DARK_SQUARE_COLOR = 0x00EEEED2,
				LIGHT_SQUARE_COLOR = 0x00769656,
				YELLOW = 0x00FFFF00,
				MARGIN_COLOR = 0x00955C3E
};
extern struct Screen screen;
/******************************************************************************
 * Stuff pertaining to bitmaps of characters and pieces
******************************************************************************/
static BMP chars;
static u8 *chars_data;
static int char_width = 15;
static int line_skip = 28; // line_skip should be more than the height of a letter;

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

// New stuff
int draw_char(u32 screen_top, u32 screen_left, char c);
int draw_string(u32 screen_top, u32 screen_left, char *str);
// int update_times(struct Player_times);


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

	if( (err = read_bitmap_file_2("Letters.bmp", &chars, chars_data, CHARS_DATA_SIZE)) != 0){
		WHERE xil_printf("Unsuccessful load of Letters.bmp\n");
		return err;
	}
	if( (err = read_bitmap_file_2("CP.bmp", &pieces, pieces_data, PIECE_DATA_SIZE)) != 0){
		WHERE xil_printf("Unsuccessful load of ChessPieces.bmp\n");
		return err;
	}

	offsets[PAWN] = 19; //19
	offsets[ROOK] = 13; //13
	offsets[QUEEN] = 1;
	offsets[KNIGHT] = 8;
	offsets[KING] = 8;
	offsets[BISHOP] = 6;
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

	return draw_partial_bitmap_2( screen_top, screen_left,
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
			if( c >= 'W') c -= 1;
			if( cursor_left + char_width + 2 >= screen.w){
				cursor_left = screen_left;
				cursor_top += line_skip;
			}
			if( (err =draw_char(cursor_top, cursor_left, c)) != 0){
				WHERE xil_printf("Could not draw char %c\n");
				return err;
			} else {
				cursor_left += char_width;
			}
		} else {
			xil_printf("%s(): Unknown char\n", __FUNCTION__);
		}

	}
	return 0;
}

/******************************************************************************
 * Something like this to draw the information on the screen
******************************************************************************/
int draw_information(struct GameInfo *gi)
{
	int some_top = 0, some_left = 0;
	draw_string(some_top, some_left, gi->player1);
	draw_string(some_top, some_left, gi->player2);
	// ...
	draw_string(some_top, some_left, gi->event);

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

	u32 screen_top = bd.top + rank_to_pixel(rank) + v_offset;
	u32 screen_left = bd.left + file_to_pixel(file) + offsets[type];

	return draw_partial_bitmap_2( screen_top , screen_left,
								bmp_top,    bmp_left,
								bmp_bottom, bmp_right,
								&pieces, pieces_data);
}

/******************************************************************************
 * Color a square in the screen
******************************************************************************/
int color_square(File file, Rank rank, u32 color)
{
	u32 screen_top = bd.top + rank_to_pixel(rank);
	u32 screen_left = bd.left + file_to_pixel(file);
	return draw_square(screen_top, screen_left, 
						bd.square_size, bd.square_size, color);
}

/******************************************************************************
 *
******************************************************************************/
int clear_square(File file, Rank rank)
{
	u32 color;
	if( (rank + file) % 2 == 0 )
		color = DARK_SQUARE_COLOR;
	else 
		color = LIGHT_SQUARE_COLOR;

	return color_square(file, rank, color);
}

/******************************************************************************
 * Set parameters in chessboard data.
******************************************************************************/
int set_chess_board_params(int top, int left, int square_size, u32 margin)
{
	bd.top = top;
	bd.left = left;
	bd.square_size = square_size;
	bd.margin = margin;
	return 0;
}


/******************************************************************************
 * Like what I had in the example
 * draw bigger square in the back, then draw the pieces
******************************************************************************/
 int draw_chess_board()
{
	int file = A;
	int rank = R1;
	int err;
	set_background_color(0x00666666);

	// Dessiner la marge (un carré plus gros en arriere du board).
	draw_square(bd.top - bd.margin,bd.left - bd.margin,
			8*bd.square_size + 2*bd.margin, 8*bd.square_size + 2*bd.margin,
			MARGIN_COLOR);

	// Dessiner les carrés verts et blancs
	for( file = A; file <= H; file++)
		for( rank = R1; rank <= R8; rank++)
			if((err = clear_square(file,rank)) != 0) return err;

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


	// TESTS de la fonction move_piece

	struct Move mv;
	mv.c = WHITE;
	mv.t = PAWN;
	mv.o_file = E;
	mv.o_rank = R2;
	mv.d_file = E;
	mv.d_rank = R4;

	BoardDisplay_move_piece(&mv);
	draw_square(200, bd.left + 8*bd.square_size + bd.margin + 10, 250,500, 0);
	draw_string(200, bd.left + 8*bd.square_size + bd.margin + 10,
				"1. e4");

	mv.c = BLACK;
	mv.t = PAWN;
	mv.o_file = E;
	mv.o_rank = R7;
	mv.d_file = E;
	mv.d_rank = R5;

	BoardDisplay_move_piece(&mv);
	draw_string(200, bd.left + 8*bd.square_size + bd.margin + 10,
				"1. e4  e5");

	mv.c = WHITE;
	mv.t = KNIGHT;
	mv.o_file = G;
	mv.o_rank = R1;
	mv.d_file = F;
	mv.d_rank = R3;

	BoardDisplay_move_piece(&mv);
	draw_string(200, bd.left + 8*bd.square_size + bd.margin + 10,
				"1. e4  e5\n2. Nf3");
#if 0
#endif
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
	static int first_move = 1;
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
	
	/*
	 * Remember the last move for the next one.
	 */
	first_move = 0;
	last = *move;

	return 0;
}


/******************************************************************************
 *
******************************************************************************/
u32 file_to_pixel(int file)
{
	return file * bd.square_size;
}

/******************************************************************************
 *
******************************************************************************/
u32 rank_to_pixel(int rank)
{
	return (7 - rank) * bd.square_size;
}
