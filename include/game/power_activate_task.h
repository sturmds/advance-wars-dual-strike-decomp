#ifndef GAME_POWER_ACTIVATE_TASK_H
#define GAME_POWER_ACTIVATE_TASK_H

#include "types.h"

/* Task started for an activated power (Power_StartTasks). The first 0x54
 * bytes belong to the task system (Task_Create). */
typedef struct PowerActivateTask {
  u8 pad_00[0x54];
  int player;
  int power;
} PowerActivateTask;

#endif
