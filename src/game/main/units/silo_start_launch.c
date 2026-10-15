#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"

extern const TaskScriptCmd data_02169344[];

/* Work area of the silo launch task (script data_02169344). */
typedef struct SiloLaunchTask {
  u8 pad_00[0x2c];
  int targetX;
  int targetY;
  u8 pad_34[0x4a - 0x34];
  u16 frames;
  u8 pad_4c[0x64 - 0x4c];
  u16 siloX;
  u16 siloY;
} SiloLaunchTask;

/* Starts the launch of the silo at (siloX, siloY) at the target tile
 * (targetX, targetY). parentOrList below 8 is a task list level, otherwise
 * the parent task. */
void Silo_StartLaunch(int siloX, int siloY, int targetX, int targetY,
                      void* parentOrList) {
  SiloLaunchTask* task;

  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.silosLaunched < 999999) {
      gPlayRecords.silosLaunched++;
    }
  }
  if ((int)parentOrList < TASK_LEVELS) {
    task = (SiloLaunchTask*)Task_Create(data_02169344, parentOrList);
  } else {
    task = (SiloLaunchTask*)Task_CreateChild(data_02169344, parentOrList);
  }
  task->siloX = siloX;
  task->siloY = siloY;
  task->targetX = targetX;
  task->targetY = targetY;
  task->frames = 30;
}
