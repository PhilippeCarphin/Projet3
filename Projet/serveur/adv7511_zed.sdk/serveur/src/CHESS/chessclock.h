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
 * Appelé lorsqu'une nouvelle partie est débutée pour mettre les temps aux
 * valeurs intiales.
*******************************************************************************/
int chessclock_init(GameInfo *gi);

/*******************************************************************************
 * Appelé lorsqu'une nouvelle partie est débutée pour mettre les temps aux
 * valeurs intiales.
*******************************************************************************/
int chessclock_overtime_reached(GameInfo *gi);

/*******************************************************************************
 * Sera appelé à chaque tour pour ajouter l'increment approprié (increment ou
 * overtime_increment) qu'on est en temps normal ou en overtime.
*******************************************************************************/
int chessclock_add_increment(GameInfo *gi, PlayerID player);

#endif
