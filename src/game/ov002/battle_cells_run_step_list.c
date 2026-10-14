#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleScreenTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_hit_list.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

#define MENU ((MenuState*)&gMenuState)
/* The second hit list of a side. Needed to match: the cast to its own type, and the later
 * blocks read the list through this full expression so that the cursor is reloaded. */
#define LIST(side) ((BattleHitList*)&MENU->battle.stepLists[side])
#define DUE_CELL_TASK(side, cursor)                                            \
  ((BattleCellTask*)MENU->battle.unk_310[side][LIST(side)->flags[cursor].cell] \
       .unk_00)
#define SIDE_COLOUR(sprite, side) (((u16(*)[2])MENU->colours)[sprite][side])

/* BATTLE_SIDE_UNIT_TYPE(side), read from the side's record. Needed to match: it keeps
 * `other` in the stack slot before `other * 12`. */
#define SIDE_UNIT_TYPE(side) ((&Ov002_gBattleScene->sides[side].hpBefore)[-1])

void BattleCell_ApplyStep(int side, int index);
void Battle_StartCellSprite(int side, int cell, int param);
void Battle_StartCellSpriteOnSlot(int side, int cell, int unk_3, int unk_6,
                                  int index);
int BattleCellTask_GetCellOffsetX(BattleCellTask* task, int unk_3);
int BattleCellTask_GetCellOffsetY(BattleCellTask* task, int unk_3);

/* Makes the hit sprite at the cell task's position. Needed to match: the locals are declared
 * at function scope and passed in, each use has its own scale variable, the last use has its
 * own side variable, and the side passes through the scale variable first. */
#define CREATE_HIT_SPRITE(scaleVar, sideVar, offsetYVar, offsetXVar, task,  \
                          arg570, arg4e0)                                   \
  do {                                                                      \
    scaleVar = (task)->side;                                                \
    sideVar = scaleVar;                                                     \
    scaleVar = (task)->scale;                                               \
    offsetYVar = BattleCellTask_GetCellOffsetY((task), (arg570));           \
    offsetXVar = BattleCellTask_GetCellOffsetX((task), (arg4e0));           \
    BattleSpriteTask_CreateIfSet(                                           \
        MENU->battle.hitSpriteTables[(task)->side], 0, (task)->posX,        \
        (task)->posY, (task)->posZ, offsetYVar, (task)->angle + offsetXVar, \
        -1, scaleVar, scaleVar, SIDE_COLOUR(5, sideVar), 8, 1, sideVar,     \
        (task)->hasPos2, (task)->pos2X, (task)->pos2Y, (task)->pos2Z, 8);   \
  } while (0)

/* Runs the entries of each side's second hit list that are due on this frame, like
 * BattleCells_RunSequence. When the other side is artillery or a battleship (or the entry has
 * unk_7 set) it only calls BattleCell_ApplyStep. Otherwise it hands the entry to
 * Battle_StartCellSpriteOnSlot or Battle_StartCellSprite when the other side has battle.unk_48, or makes a
 * hit sprite at the cell and sets step 4 in the cell task. */
void BattleCells_RunStepList(BattleScreenTask* screen) {
  u16 scale;
  u16 entryScale;
  u16 firstCellScale;
  int other;
  int side;
  int cellSide;
  int firstCellSide;
  BattleHitFlags* flags;
  int cursor;
  int offsetY;
  BattleHitList* hitList;
  int offsetX;
  BattleCellTask* cellTask;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    other = (side + 1) & 1;
    hitList = (BattleHitList*)&MENU->battle.stepLists[side];
    while (hitList->cursor != hitList->count &&
           screen->frame == hitList->key[hitList->cursor]) {
      flags = &hitList->flags[hitList->cursor];
      if (flags->unk_7 || SIDE_UNIT_TYPE(other) == UNIT_ARTILLERY ||
          SIDE_UNIT_TYPE(other) == UNIT_BATTLESHIP) {
        BattleCell_ApplyStep(side, hitList->cursor);
      } else if (MENU->battle.unk_27c[side]->unk_48 == 0) {
        if (MENU->battle.unk_48[other] != 0) {
          Battle_StartCellSpriteOnSlot(
              side, hitList->flags[hitList->cursor].cell,
              hitList->flags[hitList->cursor].unk_3,
              hitList->flags[hitList->cursor].unk_6, hitList->cursor);
        } else {
          if (flags->unk_6) {
            cellTask = DUE_CELL_TASK(side, LIST(side)->cursor);
            CREATE_HIT_SPRITE(scale, cellSide, offsetY, offsetX, cellTask, -1,
                              -1);
          } else {
            cursor = LIST(side)->cursor;
            cellTask = DUE_CELL_TASK(side, cursor);
            CREATE_HIT_SPRITE(entryScale, cellSide, offsetY, offsetX, cellTask,
                              LIST(side)->flags[cursor].unk_3,
                              LIST(side)->flags[cursor].unk_3);
          }
          cellTask = DUE_CELL_TASK(side, LIST(side)->cursor);
          cellTask->steps[cellTask->stepIndex] = 4;
          cellTask->stepIndex++;
          if (cellTask->stepIndex == 8) {
            cellTask->stepIndex = 0;
          }
          BattleCell_ApplyStep(side, LIST(side)->cursor);
        }
      } else {
        if (MENU->battle.unk_48[other] != 0) {
          Battle_StartCellSprite(side, flags->unk_3, hitList->cursor);
        } else {
          cellTask = (BattleCellTask*)MENU->battle.unk_310[side][0].unk_00;
          CREATE_HIT_SPRITE(
              firstCellScale, firstCellSide, offsetY, offsetX, cellTask,
              LIST(side)->flags[LIST(side)->cursor].unk_3, flags->unk_3);
          cellTask = (BattleCellTask*)MENU->battle.unk_310[side][0].unk_00;
          cellTask->steps[cellTask->stepIndex] = 4;
          cellTask->stepIndex++;
          if (cellTask->stepIndex == 8) {
            cellTask->stepIndex = 0;
          }
          BattleCell_ApplyStep(side, LIST(side)->cursor);
        }
      }
      LIST(side)->cursor++;
    }
  }
}
