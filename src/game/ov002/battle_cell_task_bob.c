#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"

#define MENU ((MenuState*)&gMenuState)

/* The original has this code once in each branch below. */
#define SET_SPRITE_ANGLES(task)                                 \
  do {                                                          \
    CELL_TASK_ENTRY(task).sprites[0]->unk_4a = (task)->angle;   \
    if (CELL_TASK_ENTRY(task).sprites[1] != NULL) {             \
      CELL_TASK_ENTRY(task).sprites[1]->unk_4a = (task)->angle; \
    }                                                           \
    if (CELL_TASK_ENTRY(task).sprites[2] != NULL) {             \
      CELL_TASK_ENTRY(task).sprites[2]->unk_4a = (task)->angle; \
    }                                                           \
    if (CELL_TASK_ENTRY(task).sprites[3] != NULL) {             \
      CELL_TASK_ENTRY(task).sprites[3]->unk_4a = (task)->angle; \
    }                                                           \
  } while (0)

BOOL Battle_IsSideAir(int side);
int MainLoop_GetFrameCount(void);
int Battle_BobStepLong(int phase);
int Battle_BobStep(int phase);

/* Bobs the sprites of an HP cell on an air side: adds an offset by the phase to the task's
 * angle, copies it to the sprites, and advances the phase every second frame. */
void BattleCellTask_Bob(BattleCellTask* task) {
  if (!Battle_IsSideAir(task->side)) {
    return;
  }
  if (task->flag3) {
    return;
  }
  if (MENU->battle.unk_27c[task->side]->unk_48 == 0) {
    if (task->phase == 0x90) {
      task->phase = 0;
    }
    task->angle += Battle_BobStep(task->phase);
    SET_SPRITE_ANGLES(task);
  } else {
    if (task->phase == 0xc0) {
      task->phase = 0;
    }
    task->angle += Battle_BobStepLong(task->phase);
    SET_SPRITE_ANGLES(task);
  }
  if (MainLoop_GetFrameCount() & 1) {
    task->phase++;
  }
}
