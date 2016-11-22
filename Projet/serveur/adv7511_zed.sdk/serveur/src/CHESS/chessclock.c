#include "chessclock.h"
#include "chessboard.h"





static struct PlayerTimes pt;
extern bool player1Turn;

/*******************************************************************************
 *
*******************************************************************************/
int chessclock_1sec_callback()
{
	// Figure out whose time we will modify.
	int *tm = (player1Turn == true ? &(pt.whiteTime) : &(pt.blackTime));

	// Reduce the time
	*tm -= 1;

	// Update the display
	BoardDisplay_update_times(player1Turn, *tm);

	return 0;
}

/*******************************************************************************
 * Appelé lorsqu'une nouvelle partie est débutée pour mettre les temps aux
 * valeurs intiales.
		 * struct {
				int time;
				int increment;
				int limit;
				int overtime;
				int overtime_increment;
			} timer_format;
*******************************************************************************/
int chessclock_init(GameInfo *gi)
{
	pt.whiteTime = gi->timer_format.time;
	pt.blackTime = gi->timer_format.time;
	return 0;
}

/*******************************************************************************
 * Appelé lorsque la limite de coup est atteinte, l'overTime est ajouté
*******************************************************************************/
int chessclock_overtime_reached(GameInfo *gi)
{
	pt.whiteTime += gi->timer_format.overtime;
	pt.blackTime += gi->timer_format.overtime;
	return 0;
}

/*******************************************************************************
 * Sera appelé à chaque tour pour ajouter l'increment approprié (increment ou
 * overtime_increment) qu'on est en temps normal ou en overtime.
*******************************************************************************/
int chessclock_add_increment(GameInfo *gi, int player /* WHITE or BLACK */)
{
	int *tm = (player1Turn == true ? &(pt.whiteTime) : &(pt.blackTime));

	*tm += gi->timer_format.increment;

	draw_player_time(player1Turn, *tm);

	return 0;
}
