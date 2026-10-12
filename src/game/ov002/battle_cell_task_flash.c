#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSpriteTask.h"

/* unk_56 and unk_58 of a cell sprite: base on the left side, base + 2 on the right, as
 * BattleCellTask_RestartSprite passes them. Probably palette numbers. */
#define SIDE_PALETTE(task, base) ((task)->side == 0 ? (base) : (base) + 2)

/* Sets unk_56 and unk_58 of the first three sprites of the task's cell. The original has this
 * code once in each branch below. */
#define SET_SPRITE_PALETTES(task, base)                                    \
  do {                                                                     \
    CELL_TASK_ENTRY(task).sprites[0]->unk_56 = SIDE_PALETTE(task, base);   \
    CELL_TASK_ENTRY(task).sprites[0]->unk_58 = SIDE_PALETTE(task, base);   \
    if (CELL_TASK_ENTRY(task).sprites[1] != NULL) {                        \
      CELL_TASK_ENTRY(task).sprites[1]->unk_56 = SIDE_PALETTE(task, base); \
      CELL_TASK_ENTRY(task).sprites[1]->unk_58 = SIDE_PALETTE(task, base); \
    }                                                                      \
    if (CELL_TASK_ENTRY(task).sprites[2] != NULL) {                        \
      CELL_TASK_ENTRY(task).sprites[2]->unk_56 = SIDE_PALETTE(task, base); \
      CELL_TASK_ENTRY(task).sprites[2]->unk_58 = SIDE_PALETTE(task, base); \
    }                                                                      \
  } while (0)

/* Flashes the sprites of an HP cell while flashing is set: each flash uses palette 4 (6 on the
 * right side) for four frames, then the normal palette for two. After flashCount flashes it
 * clears flashing and puts the normal palette back. */
void BattleCellTask_Flash(BattleCellTask* task) {
  if (!task->flashing) {
    return;
  }
  if (task->flashTimer == 6) {
    task->flashTimer = 0;
    task->flashCount--;
    if (task->flashCount == 0) {
      task->flashing = 0;
      SET_SPRITE_PALETTES(task, 0);
      return;
    }
  }
  if (task->flashTimer < 4) {
    SET_SPRITE_PALETTES(task, 4);
  } else {
    SET_SPRITE_PALETTES(task, 0);
  }
  task->flashTimer++;
}
