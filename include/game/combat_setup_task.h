#ifndef GAME_COMBAT_SETUP_TASK_H
#define GAME_COMBAT_SETUP_TASK_H

#include "types.h"

/* Task of the Combat setup screens (link play). The first 0x4e bytes belong to
 * the task system (Task_Create). */
typedef struct CombatSetupTask {
    u8 pad_00[0x4e];
    s16 unk_4e;                        /* nonzero: func_02088478 keeps the settings already in data_02160ac0 */
} CombatSetupTask;

#endif
