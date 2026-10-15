#ifndef GAME_BATTLECELLTASK_H
#define GAME_BATTLECELLTASK_H

#include "types.h"
#include "game/menu_state.h"

struct BattleSpriteTask;

/* Task that moves the sprites of one HP cell of the battle animation in ov002.
 * It is made from the script at data_ov002_0236963c and stored in BattleEntry.unk_00.
 * The first 0x30 bytes belong to the task system. mwcc packs bit fields from the low end. */
typedef struct BattleCellTask {
  u8 pad_00[0x2c];
  struct BattleSpriteTask*
      shadow; /* sprite from battle.unk_40 that follows the cell (BattleCellTask_MakeMatchupSprites) */
  u8 steps[8]; /* what the cell does at each step, indexed by stepIndex */
  s16 posX;    /* position of the sprites */
  s16 posY;
  s16 posZ;
  s16 pos2X; /* second position, used when hasPos2 is set */
  s16 pos2Y;
  s16 pos2Z;
  u16 scale; /* starts at 0x100 */
  s16 angle; /* bobbing offset, copied to the sprites; a guess */
  s16 velX; /* 8.8 fixed point, added to the positions each frame while moving is set */
  s16 velY;
  s16 accelX; /* 8.8 fixed point, added to velX each frame */
  s16 accelY;
  s16 unk_50; /* cleared by BattleCellTask_Reset */
  u8 pad_52[0x54 - 0x52];
  u8 flashCount; /* flashes left while flashing is set */
  u8 flashTimer; /* frame within one flash, 0 to 6 */
  u8 phase;      /* frame phase of the bobbing, starts at cell * 16 */
  u8 unk_57;
  u8 hasPos2 : 1;
  u8 pos2Index : 7;
  u8 moving : 1;
  u8 hasShadow : 1;
  u8 flashing : 1;
  u8 flag3
      : 1; /* the fighter motion steps run only while set; BattleCellTask_Bob skips while set */
  u8 side : 1;
  u8 cell : 3;
  u8 unk_5a_lo : 4;
  u8 stepIndex : 4; /* index into steps; wraps to 0 at 8 */
} BattleCellTask;

void BattleCellTask_Reset(BattleCellTask* task);
int BattleCellTask_GetOffsetY(BattleCellTask* task);
int BattleCellTask_GetOffsetX(BattleCellTask* task);
BattleCellTask* BattleCellTask_Create(int side, int cell, int pos2Index);
void BattleCellTask_DestroySprites(BattleCellTask* task);
void BattleCellTask_RestartSprite(BattleCellTask* task);
void BattleCellTask_Bob(BattleCellTask* task);
void BattleCellTask_Flash(BattleCellTask* task);
void BattleCellTask_MakeMatchupSprites(BattleCellTask* task);
void BattleCellTask_Move(BattleCellTask* task);
void BattleCellTask_RemakeSprites(BattleCellTask* task);
void BattleCellTask_StartMove(BattleCellTask* task);
void BattleCellTask_RemakeSpritesWithEffect(BattleCellTask* task);
void BattleCellTask_StartFlash(BattleCellTask* task);
void BattleCellTask_UpdateShadow(BattleCellTask* task);

/* Needed to match: the compiler folds the offset of the array into the field offset only when
 * the address is built from gMenuState's start with the indices added to it. */
#define BATTLE_CELL_ENTRY(side, cell)                                  \
  (*(BattleEntry*)((u8*)&gMenuState +                                  \
                       (side) * sizeof(gMenuState.battle.unk_310[0]) + \
                       (cell) * sizeof(BattleEntry) + (u32) &          \
                   ((MenuState*)0)->battle.unk_310))

/* The entry of the side and cell a cell task belongs to. */
#define CELL_TASK_ENTRY(task) BATTLE_CELL_ENTRY((task)->side, (task)->cell)

#endif
