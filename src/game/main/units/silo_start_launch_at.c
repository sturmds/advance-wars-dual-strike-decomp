#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const TaskScriptCmd data_021693bc[];

/* Work area of the task of script data_021693bc. */
typedef struct SiloTask {
  u8 pad_00[0x64];
  u16 siloX;
  u16 siloY;
} SiloTask;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Counts a silo launch for a human current player and starts the task of
 * script data_021693bc for the silo at (siloX, siloY). Like Silo_StartLaunch,
 * parentOrList below TASK_LEVELS means a top-level task; it then goes in
 * list 3. Otherwise it is the parent task. */
void func_020dda8c(int siloX, int siloY, int parentOrList) {
  SiloTask* task;

  if (GAME->players[GAME->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.silosLaunched < 999999) {
      gPlayRecords.silosLaunched++;
    }
  }
  if (parentOrList < TASK_LEVELS) {
    task = (SiloTask*)Task_Create(data_021693bc, TASK_LIST(3));
  } else {
    task = (SiloTask*)Task_CreateChild(data_021693bc, (Task*)parentOrList);
  }
  task->siloX = siloX;
  task->siloY = siloY;
}
