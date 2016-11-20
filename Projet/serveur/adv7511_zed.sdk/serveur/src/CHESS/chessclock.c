#include "chessclock.h"
#include "chessboard.h"



#if 0

static struct PlayerTimes pt;
/*******************************************************************************
 *
*******************************************************************************/
int chessclock_1sec_callback()
{
	// reduce the correct player's time by 1 sec

	//update display
		// Need to create
		BoardDisplay_update_times(...)
		{
			draw_square(...) // erase the time

			// time is in seconds, so we need to figure out
			char time_string[9]; // hh:mm:ss (8 chars + '\0' at the end)

			// after this call time_string contains the time in string format ready for display
			int_to_time_string(time, time_string);

			draw_string(vertical_position, horizontal_position, time_string);

			cf_hdmi_send_buffer();// Send the screen buffer to the screen.
		}
		and call it.

	return;
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
}

/*******************************************************************************
 * Appelé lorsque la limite de coup est atteinte, l'overTime est ajouté
*******************************************************************************/
int chessclock_overtime_reached(GameInfo *gi)
{
	pt.whiteTime += gi->timer_format.overtime;
	pt.blackTime += gi->timer_format.overtime;
}

/*******************************************************************************
 * Sera appelé à chaque tour pour ajouter l'increment approprié (increment ou
 * overtime_increment) qu'on est en temps normal ou en overtime.
*******************************************************************************/
int chesscloak_add_increment(GameInfo *gi, int player /* WHITE or BLACK */)
{
	if(player == 1)
	{
		pt.whiteTime += gi->timer_format.increment;
	}
	else
	{
		pt.blackTime += gi->timer_format.increment;
	}

}
#endif
