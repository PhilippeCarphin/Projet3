#ifndef _CHESS_CLOCK_H_
#define _CHESS_CLOCK_H_
#include "chessboard_rest_protocol.h"

struct PlayerTimes {
	int whiteTime;
	int blackTime;
};

/*******************************************************************************
 *
*******************************************************************************/
int chessclock_1sec_callback();

/*******************************************************************************
 * Appel� lorsqu'une nouvelle partie est d�but�e pour mettre les temps aux
 * valeurs intiales.
*******************************************************************************/
int chessclock_init(GameInfo *gi);

/*******************************************************************************
 * Appel� lorsqu'une nouvelle partie est d�but�e pour mettre les temps aux
 * valeurs intiales.
*******************************************************************************/
int chessclock_overtime_reached(GameInfo *gi);

/*******************************************************************************
 * Sera appel� � chaque tour pour ajouter l'increment appropri� (increment ou
 * overtime_increment) qu'on est en temps normal ou en overtime.
*******************************************************************************/
int chessclock_add_increment(GameInfo *gi, PlayerID player);

#endif
