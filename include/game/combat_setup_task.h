#ifndef GAME_COMBAT_SETUP_TASK_H
#define GAME_COMBAT_SETUP_TASK_H

#include "types.h"

/* The Combat setup screens (link play) use their Task with these fields at 0x4c,
 * where Task has unk_4c. Cast to Task * for the task functions. The first 0x4c
 * bytes belong to the task system (Task_Create). */
typedef struct CombatSetupTask {
  u8 pad_00[0x4c];
  s16 waitFrames; /* frames func_02088b3c has waited for the other players */
  s16 unk_4e; /* nonzero: func_02088478 keeps the settings already in data_02160ac0 */
} CombatSetupTask;

#endif
