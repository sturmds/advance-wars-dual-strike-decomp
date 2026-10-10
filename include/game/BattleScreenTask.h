#ifndef GAME_BATTLESCREENTASK_H
#define GAME_BATTLESCREENTASK_H

#include "types.h"

/* The task that runs the battle screen in ov002 (func_ov002_023586f8 is its update). The
 * first 0x2a bytes belong to the task system. */
typedef struct BattleScreenTask {
    u8 pad_00[0x2a];
    u16 frame;                         /* screen frame; BattleHitList.key is compared with it */
    u8 fighterStarted[2];              /* per side: a fighter task was started */
} BattleScreenTask;

#endif
