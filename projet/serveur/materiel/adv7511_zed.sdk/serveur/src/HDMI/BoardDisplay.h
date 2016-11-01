#ifndef _BOARD_DISPLAY_H_
#define _BOARD_DISPLAY_H_

/******************************************************************************
 * Stuff for the chessboard coordinates and piece identification
******************************************************************************/
typedef enum File {A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7} File;
typedef enum Rank {R1 = 0, R2 = 1, R3 = 2, R4 = 3, R5 = 4, R6 = 5, R7 = 6, R8 = 7}Rank;
typedef enum PieceType {PAWN = 0, KING = 1, QUEEN = 2, ROOK = 3, BISHOP = 4, KNIGHT = 5}PieceType;
typedef enum PieceColor {WHITE = 0, BLACK = 2} PieceColor;


struct Move{
	PieceType t;
	PieceColor c;
	File o_file;
	Rank o_rank;
	File d_file;
	Rank d_rank;
};

// TEMPORARY:
struct GameInfo{
	char player1[10];
	char player2[10];
	char event[10];
};

// TEMPORARY:
struct PlayerTimes{
	char player1[10];
	char player2[10];
};

int draw_chess_board();
int boardDisplay_init();

int set_chess_board_params(int top, int left, int square_size, u32 margin);

int move_piece(struct Move *move);

int draw_information(struct GameInfo *gi);

int update_times(struct PlayerTimes *pt);

#endif //_BOARD_DISPLAY_H_


// QUESTION:    I think once we've made our modification, we should call
//				DDRVideoWr() or at least signal the cf_hdmi module that it 
//              needs to resend screen data over the HDMI interface

// TODO :       Find a bitmap file for chess pieces.  This one is nice
//              http://www.vectorhq.com/vector/chess-symbols-set-9758
//              but we should convert it to 32 bit uncompressed bitmap.
//              and adjust the enums to reflect the placement of the pieces,
//              also scale the image.

// TODO :       Find or make a bitmap for a font.  I.E. take the string
//               !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
//              and paste it in a fixed width font in an image editor and
//              save a bitmap of this.  

// TODO :       Make this code compile and modify it as needed so that it 
//              works with the ChessBoard module.  Like maybe they could both
//              use the same enums, or maybe not.

// TODO :       Add error checks everywhere; the drawHDMI functions all return
//              0 for success and -1 for failure.  This should be done here too 
//              with xil_printf()'s where pertinent.

// And of course, as the Gordon Ramsay says "Taste taste taste" but we're going
// to pretend that he says "Test test test"



// Utilisation:
/*
ChessBoard_playMove(...)
{
    // Verifier si le move est legal
   
    struct Move mv;
    mv.o_file = ...
    
    move_piece(&move);
    // Ou peut etre juste avoir
    move_piece(type, color, orig_file, orig_rank, dst_file, dst_rank);
    

     * Faire un appel a une fonction REST pour qu'il fasse un appel HTTP pour
     * repondre a la tablette, ou retourner quelque chose pour dire que le 
     * move est légal.

}*/

