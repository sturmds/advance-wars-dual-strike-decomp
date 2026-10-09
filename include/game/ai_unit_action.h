#ifndef GAME_AI_UNIT_ACTION_H
#define GAME_AI_UNIT_ACTION_H

#include "types.h"

/* State of the AI turn, gAiTurnState->state. AiTurn_Update
 * switches on it. */
#define AI_STATE_START       0  /* set up the turn */
#define AI_STATE_RUN_PHASE   1  /* run the unit action step of the current phase */
#define AI_STATE_RUN_QUEUE   2  /* call gAiQueueRoutine for each queue entry */
#define AI_STATE_SHOW_ACTION 3  /* a queued unit acted; name is a guess */
#define AI_STATE_END_ACTIONS 4  /* unit actions done */

/* Queue entry that is not a unit index: the step runs once. */
#define AI_QUEUE_RUN_ONCE 0x40

/* gAiTurnState->flags, as what an indirect unit may shoot besides units. */
typedef struct AiTargetFlags {
    u8 unk_0 : 2;
    u8 attackTiles : 1;                /* tiles with hp (Unk_02080440_48.tileHp) */
    u8 attackObjects : 1;              /* MapObject */
    u8 unk_4 : 4;
} AiTargetFlags;

typedef struct AiTurnState {
    u8 state;                          /* AI_STATE_* */
    u8 unk_01;
    u8 phase;                          /* index into the unit phase tables */
    u8 roll;                           /* 0-99, rolled for each queued unit */
    u8 pad_04;
    u8 unk_05;
    u8 pad_06;
    u8 flags;                          /* bits 0-1: cleared for each queued unit; 2, 3: see AiTurn_Start and AiTargetFlags */
    u8 unk_08;
} AiTurnState;

/* The unit action steps (pointer tables at 0x02161a68 and 0x02161ae8)
 * fill a queue of unit indices, ended by 0, and pick a routine to run
 * on each entry. */
extern AiTurnState *gAiTurnState;

#define AI_TARGET_FLAGS ((AiTargetFlags *)&gAiTurnState->flags)
extern u8 *gAiQueueCursor;            /* write position in gAiQueue */
extern void (*gAiQueueRoutine)(void); /* run on each entry */
extern u8 gAiQueue[];

#endif
