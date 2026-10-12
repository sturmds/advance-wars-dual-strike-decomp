#include "types.h"
#include "game/battle_fighter_task.h"

/* What BattleSideData.cellPositions points to, read at the offsets 0x22 and 0x24. */
typedef struct CueSize {
  u8 pad_00[0x22];
  s16 width;
  s16 height;
} CueSize;

#define CUE_SPAWN 5

int Battle_GetMatchupValue(int side);
void func_ov002_0235643c(const u32* table, int a1, int a2, int a3, int a4,
                         int a5, int a6, int a7, int a8, int a9, int a10,
                         int a11, int a12, int a13);
void func_ov002_02357e5c(int side, int a1);
void Task_ResumeScript(void* task);

/* Needed to match: the original adds the offset into gMenuState at run time. */
#define MENU_WORD(offset, side) \
  (*(u32*)((u8*)&gMenuState + (side) * 4 + (offset)))
#define SPRITE_TABLE(side) ((const u32*)MENU_WORD(0x40, side))

/* A task that waits for its sprite task to reach cue 5, then starts another
 * sprite task at the fighter's place. A guess from the fields it reads. */
void Battle_UpdateCueSpawner(BattleFighterTask* task) {
  int side;
  int scale;
  CueSize* size;
  BattleCellTask* fighter;

  if (task->sprite->unk_5b == CUE_SPAWN) {
    task->sprite->unk_5b = 0;
    side = task->side;
    if (SPRITE_TABLE(side) != 0) {
      fighter = task->fighter;
      scale = task->scale;
      size = (CueSize*)task->cells;
      func_ov002_0235643c(
          SPRITE_TABLE(task->side), Battle_GetMatchupValue(side), fighter->posX,
          fighter->posY, fighter->posZ, size->width * scale >> 8,
          size->height * scale >> 8, -1, scale, scale,
          *(u16*)((u32)&gMenuState + side * 2 + 12), 8, 0, side);
    }
    if (MENU_WORD(0x48, task->side) != 0) {
      func_ov002_02357e5c(task->side, task->step);
    }
    task->step++;
  }
  if (task->sprite->unk_5c != 0) {
    Task_ResumeScript(task);
  }
}
