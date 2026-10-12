#ifndef GAME_CO_SWAP_H
#define GAME_CO_SWAP_H

#include "game/task.h"

/* The task of the CO swap cut-in (script data_0215c764, started for
 * UnitAction_SwapCo). The front CO's picture sits in sprite list 2, the back
 * CO's in list 3, and task->unk_58 title letters in sprite slots from 0. */

/* The frame counter shares its slot with Task.unk_4c. */
#define CO_SWAP_FRAME(task) (*(s16*)&(task)->unk_4c)
/* How far each CO picture sits from its resting x, kept in Task bytes
 * 0x2c-0x2f. */
#define CO_SWAP_OFFSET(task) (*(int*)(task)->pad_2c)

#define CO_SWAP_FRONT_X 72 /* resting x of the front CO's picture */
#define CO_SWAP_BACK_X 184 /* resting x of the back CO's picture */
#define CO_SWAP_CO_Y 160
#define CO_SWAP_TITLE_Y 80

#endif
