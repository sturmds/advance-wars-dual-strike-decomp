#ifndef GAME_AI_UNIT_ACTION_H
#define GAME_AI_UNIT_ACTION_H

#include "types.h"

/* State of the AI turn, first byte of *gAiTurnState. AiTurn_Update
 * switches on it. */
#define AI_STATE_RUN_QUEUE   2  /* call gAiQueueRoutine for each queue entry */
#define AI_STATE_END_ACTIONS 4  /* unit actions done */

/* Queue entry that is not a unit index: the step runs once. */
#define AI_QUEUE_RUN_ONCE 0x40

/* The unit action steps (pointer tables at 0x02161a68 and 0x02161ae8)
 * fill a queue of unit indices, ended by 0, and pick a routine to run
 * on each entry. */
extern u8 *gAiTurnState;
extern u8 *gAiQueueCursor;            /* write position in gAiQueue */
extern void (*gAiQueueRoutine)(void); /* run on each entry */
extern u8 gAiQueue[];

#endif
