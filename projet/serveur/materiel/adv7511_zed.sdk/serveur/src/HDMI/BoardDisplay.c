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
static int piece_width = 133;
static int piece_height = 204 - WHITE;

#define v_offset 10;
#define h_offset 10;
/******************************************************************************
 * Declarations of functions
******************************************************************************/
int draw_piece(PieceType type, PieceColor color, int file, int rank);
int color_square(int file, int rank, int color);
int clear_square(int file, int rank);
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

int BoardDisplay_set_image_buffers(u8 *chars_dat, u8 *pieces_dat)
{
	chars_data = chars_dat;
	pieces_data = pieces_dat;
	return 0;
}

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
		if( c == '\n')
		{
			cursor_top += line_skip;
			cursor_left = screen_left;
		}
		else if( c == ' ')
		{
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
			}

			else
			{
				cursor_left += char_width;
			}
		}
		else
		{
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
 *
******************************************************************************/
int draw_piece(PieceType type, PieceColor color, int file, int rank)
{
	/*
	 * Example calculation for finding the position of the piece in 
	 * an image assuming like this
	 *   WP WK WQ ...
	 *   BP BK BQ
	 * where the white pieces are in one row at the top and the black pieces
	 * are in the same order at the bottom.
	 */
	u32 bmp_top = color;
	u32 bmp_left = type;

	u32	bmp_bottom = bmp_top + piece_height;
	u32 bmp_right = MIN(bmp_left + 90,800);

	/*
	 * Assuming ranks are numbered from 1 to eight with the eighth rank
	 * at the top of the board on the screen.
	 */
	u32 screen_top = bd.top + rank_to_pixel(rank) + v_offset;
	u32 screen_left = bd.left + file_to_pixel(file) + h_offset;

	/* 
	 * v_offset and h_offset to make the pieces centered inside their squares
	 */
	//screen_top += v_offset;
	//screen_left += h_offset;


	return draw_partial_bitmap_2( screen_top , screen_left,
								bmp_top,    bmp_left,
								bmp_bottom, bmp_right,
								&pieces, pieces_data);
}

/******************************************************************************
 *
******************************************************************************/
int color_square(int file, int rank, int color)
{
	/*
	 * Assuming ranks are numbered from 1 to eight with the eighth rank
	 * at the top of the board on the screen.
	 */
	u32 screen_top = bd.top + rank_to_pixel(rank);
	u32 screen_left = bd.left + file_to_pixel(file);
	return draw_square(screen_top, screen_left, 
						bd.square_size, bd.square_size, color);
}

/******************************************************************************
 *
******************************************************************************/
int clear_square(int file, int rank)
{
	u32 color;
	if( (rank + file) % 2 == 0 )
		color = DARK_SQUARE_COLOR;
	else 
		color = LIGHT_SQUARE_COLOR;

	return color_square(file, rank, color);
}

/******************************************************************************
 * Read bitmap data into memory
******************************************************************************/
int boardDisplay_loadImages()
{
	if(read_bitmap_file_2("pieces_filename", &pieces, pieces_data, PIECE_DATA_SIZE))
	{
		xil_printf("%s(): Unable to read pieces bitmap",__FUNCTION__);
		return -1;
	}
	if(read_bitmap_file_2("characters_filename", &chars, chars_data, CHARS_DATA_SIZE))
	{
		xil_printf("%s(): Unable to read chars bitmap",__FUNCTION__);
		return -1;
	}
	return 0;
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
	draw_square(bd.top - bd.margin,bd.left - bd.margin,
			8*bd.square_size + 2*bd.margin, 8*bd.square_size + 2*bd.margin,
			MARGIN_COLOR);
	for( file = A; file <= H; file++)
		for( rank = R1; rank <= R8; rank++)
			if((err = clear_square(file,rank)) != 0) return err;

	// Dessiner un carré de largeur 140 et de hauteur 204
	//draw_square(500,900,140,204,YELLOW);

	// Dessiner la portion du bitmap de i = 0, j = 0 à i = 204, j = 140
	//draw_partial_bitmap_2(500,900,
	//					0,0,
	//					204,140,
	//					&pieces,pieces_data);

	// Dessiner le bitmap au complet
	// draw_full_bitmap_2(bd.top + 3*bd.square_size,bd.left, &pieces, pieces_data);

	// Dessiner le bitmap au complet
	//draw_full_bitmap_2(100, 30, &chars, chars_data);

	// draw_string(150,30, "J'arrive a dessiner des strings a l'ecran, je lis les bitmaps correctement\nmais ya quelque chose\navec la transparence que je fais pas bien.\nJe suis pas mal sur que je vais pouvoir trouver demain.\n    -Phil");
	//draw_square(0,0,100,(204-124),YELLOW);
	//draw_partial_bitmap(0,0,124,300,204,400,&pieces,pieces_data);

	draw_string(200, bd.left + 8*bd.square_size + bd.margin + 10,
			"Salut les amis, maintenant, je dessine une string dans l'ecran.  Je peux utiliser des '\\n' mais en plus, la fonction va automatiquement ajouter des sauts de ligne si on depasse le bord de l'ecran.");
	for( file = A; file <= H; file++)
	{
		if((err = draw_piece(PAWN, WHITE, file, R2)) != 0) return err;
		if((err = draw_piece(PAWN, BLACK, file, R7)) != 0) return err;
	}


	if((err = draw_piece(ROOK,   WHITE, A, R1)) != 0) return err;
	if((err = draw_piece(ROOK,   WHITE, H, R1)) != 0) return err;
	if((err = draw_piece(KNIGHT, WHITE, B, R1)) != 0) return err;
	if((err = draw_piece(KNIGHT, WHITE, G, R1)) != 0) return err;
	if((err = draw_piece(BISHOP, WHITE, C, R1)) != 0) return err;
	if((err = draw_piece(BISHOP, WHITE, F, R1)) != 0) return err;
	if((err = draw_piece(KING,   WHITE, E, R1)) != 0) return err;
	if((err = draw_piece(QUEEN,  WHITE, D, R1)) != 0) return err;

	if((err = draw_piece(ROOK,   BLACK, A, R8)) != 0) return err;
	if((err = draw_piece(ROOK,   BLACK, H, R8)) != 0) return err;
	if((err = draw_piece(KNIGHT, BLACK, B, R8)) != 0) return err;
	if((err = draw_piece(KNIGHT, BLACK, G, R8)) != 0) return err;
	if((err = draw_piece(BISHOP, BLACK, C, R8)) != 0) return err;
	if((err = draw_piece(BISHOP, BLACK, F, R8)) != 0) return err;
	if((err = draw_piece(KING,   BLACK, E, R8)) != 0) return err;
	if((err = draw_piece(QUEEN,  BLACK, D, R8)) != 0) return err;
#if 0
	if((err = draw_piece(BLACK, ROOK,   A, R8)) != 0) return err;
	if((err = draw_piece(BLACK, ROOK,   H, R8)) != 0) return err;
	if((err = draw_piece(BLACK, KNIGHT, B, R8)) != 0) return err;
	if((err = draw_piece(BLACK, KNIGHT, G, R8)) != 0) return err;
	if((err = draw_piece(BLACK, BISHOP, C, R8)) != 0) return err;
	if((err = draw_piece(BLACK, BISHOP, F, R8)) != 0) return err;
	if((err = draw_piece(BLACK, KING,   E, R8)) != 0) return err;
	if((err = draw_piece(BLACK, QUEEN,  D, R8)) != 0) return err;

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
	 * by restoring this to normal
	 */
	static int first_move = 1;
	static struct Move last;
	if( ! first_move )
	{
		if((err = clear_square(last.o_file, last.o_rank)) != 0) return err;
		if((err = clear_square(last.d_file, last.d_rank)) != 0) return err;
		if((err = draw_piece(last.t, last.c, last.d_file, last.d_rank)) != 0) return err;
	}

	if((err = color_square(move->o_file, move->o_rank, YELLOW)) != 0) return err;
	if((err = color_square(move->d_file, move->d_rank, YELLOW)) != 0) return err;
	if((err = draw_piece(move->t, move->c, move->d_file, move->d_rank)) != 0) return err;
	
	/*
	 * Remember this move so we can un-yellow the squares when we do the
	 * next move
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
