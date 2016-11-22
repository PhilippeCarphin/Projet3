#include "chessclock.h"
#include "chessboard.h"
#include "BoardDisplay.h"





static struct PlayerTimes pt;
extern TurnInfo currentTurnInfo;
static int overtime_reached = 0;

/*******************************************************************************
 *
*******************************************************************************/
int chessclock_1sec_callback()
{
	// Figure out whose time we will modify.
	int player = currentTurnInfo.turn;
	int *tm = (player == player1 ? &(pt.whiteTime) : &(pt.blackTime));

	// Reduce the time
	*tm -= 1;

	// Update the display
	BoardDisplay_update_times(player, *tm);

	return 0;
}

/*******************************************************************************
 * Appel� lorsqu'une nouvelle partie est d�but�e pour mettre les temps aux
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
 * Appel� lorsque la limite de coup est atteinte, l'overTime est ajout�
*******************************************************************************/
int chessclock_overtime_reached(GameInfo *gi)
{
	overtime_reached = 1;
	pt.whiteTime += gi->timer_format.overtime;
	pt.blackTime += gi->timer_format.overtime;
	return 0;
}

/*******************************************************************************
 * Sera appel� � chaque tour pour ajouter l'increment appropri� (increment ou
 * overtime_increment) qu'on est en temps normal ou en overtime.
*******************************************************************************/
int chessclock_add_increment(GameInfo *gi, PlayerID player)
{
	int *tm = (player == player1 ? &(pt.whiteTime) : &(pt.blackTime));

	if(!overtime_reached)
	{
		*tm += gi->timer_format.increment;
	}
	else
	{
		*tm += gi->timer_format.overtime_increment;
	}

	BoardDisplay_draw_player_time(player, *tm);

	return 0;
}
