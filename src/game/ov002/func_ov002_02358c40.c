#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleScreenTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_hit_list.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

#define MENU ((MenuState *)&gMenuState)
/* The second hit list of a side. The cast to its own type changes the code mwcc makes
 * (95% without it).
 * WIP (99.7%): the later blocks read the list through this full expression so that the compiler
 * reloads the cursor as the original does; the registers still differ. */
#define LIST(side) ((BattleHitList *)&MENU->battle.stepLists[side])
#define DUE_CELL_TASK(side, cursor) \
    ((BattleCellTask *)MENU->battle.unk_310[side][LIST(side)->flags[cursor].cell].unk_00)
#define SIDE_COLOUR(sprite, side) (((u16(*)[2])MENU->colours)[sprite][side])

void BattleCell_ApplyStep(int side, int index);
void Battle_StartCellSprite(int side, int cell, int param);
void Battle_StartCellSpriteOnSlot(int side, int cell, int unk_3, int unk_6, int index);
int BattleCellTask_GetCellOffsetX(BattleCellTask *task, int unk_3);
int BattleCellTask_GetCellOffsetY(BattleCellTask *task, int unk_3);

/* Makes the hit sprite at the cell task's position. Needed to match: its locals are declared
 * at function scope (97.4% as block locals). */
#define CREATE_HIT_SPRITE(task, arg570, arg4e0)                                                   \
    do {                                                                                          \
        cellSide = (task)->side;                                                              \
        scale = (task)->scale;                                                                \
        offsetY = BattleCellTask_GetCellOffsetY((task), (arg570));                                      \
        offsetX = BattleCellTask_GetCellOffsetX((task), (arg4e0));                                  \
        BattleSpriteTask_CreateIfSet(MENU->battle.hitSpriteTables[(task)->side], 0, (task)->posX, \
                                     (task)->posY, (task)->posZ, offsetY,                         \
                                     (task)->angle + offsetX, -1, scale, scale,               \
                                     SIDE_COLOUR(5, cellSide), 8, 1, cellSide, (task)->hasPos2,   \
                                     (task)->pos2X, (task)->pos2Y, (task)->pos2Z, 8);             \
    } while (0)

/* Runs the entries of each side's second hit list that are due on this frame, like
 * BattleCells_RunSequence. When the other side is artillery or a battleship (or the entry has
 * unk_7 set) it only calls BattleCell_ApplyStep. Otherwise it hands the entry to
 * Battle_StartCellSpriteOnSlot or Battle_StartCellSprite when the other side has battle.unk_48, or makes a
 * hit sprite at the cell and sets step 4 in the cell task. */
void func_ov002_02358c40(BattleScreenTask *screen)
{
    u16 scale;
    int other;
    int side;
    int cellSide;
    BattleHitFlags *flags;
    int cursor;
    int offsetY;
    BattleHitList *hitList;
    int offsetX;
    BattleCellTask *cellTask;

    for (side = 0; side < 2; side++) {
        other = (side + 1) & 1;
        hitList = (BattleHitList *)&MENU->battle.stepLists[side];
        while (hitList->cursor != hitList->count &&
               screen->frame == hitList->key[hitList->cursor]) {
            flags = &hitList->flags[hitList->cursor];
            if (flags->unk_7 || BATTLE_SIDE_UNIT_TYPE(other) == UNIT_ARTILLERY ||
                BATTLE_SIDE_UNIT_TYPE(other) == UNIT_BATTLESHIP) {
                BattleCell_ApplyStep(side, hitList->cursor);
            } else if (MENU->battle.unk_27c[side]->unk_48 == 0) {
                if (MENU->battle.unk_48[other] != 0) {
                    Battle_StartCellSpriteOnSlot(side, hitList->flags[hitList->cursor].cell,
                                        hitList->flags[hitList->cursor].unk_3,
                                        hitList->flags[hitList->cursor].unk_6, hitList->cursor);
                } else {
                    if (flags->unk_6) {
                        cellTask = DUE_CELL_TASK(side, LIST(side)->cursor);
                        CREATE_HIT_SPRITE(cellTask, -1, -1);
                    } else {
                        cursor = LIST(side)->cursor;
                        cellTask = DUE_CELL_TASK(side, cursor);
                        CREATE_HIT_SPRITE(cellTask, LIST(side)->flags[cursor].unk_3,
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
                    cellTask = (BattleCellTask *)MENU->battle.unk_310[side][0].unk_00;
                    CREATE_HIT_SPRITE(cellTask, LIST(side)->flags[LIST(side)->cursor].unk_3,
                                      flags->unk_3);
                    cellTask = (BattleCellTask *)MENU->battle.unk_310[side][0].unk_00;
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
