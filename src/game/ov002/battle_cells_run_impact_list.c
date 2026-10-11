/* Needed to match: with dead-assignment removal on, mwcc keeps the list
 * cursor in a register where the original reloads it. */
#pragma opt_dead_assignments off

#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleScreenTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_hit_list.h"

#define MENU ((MenuState *)&gMenuState)
#define DUE (list->flags[list->cursor])

/* Needed to match: declared with a u8 side here; the definition takes u32. */
void Battle_StartSideShake(u8 side);
int BattleCellTask_GetCellOffsetX(BattleCellTask *task, int index);
int BattleCellTask_GetCellOffsetY(BattleCellTask *task, int index);

/* Runs the entries of each side's unk_2ec list that are due on this frame. An entry with unk_6
 * clear makes a sprite at its cell (cell 0 when the side runs a fighter task) and keeps it in
 * unk_22c; one with unk_6 set gives the cell step 5. A side with a sprite table shakes
 * once per entry. "Impact" in the name is a guess from the sprite and the shake.
 * Needed to match: taskSide is read before the scale for the side argument only; the
 * other reads of task->side stay plain. */
void BattleCells_RunImpactList(BattleScreenTask *screen)
{
    int side;
    BattleHitList *list;
    BattleCellTask *task;
    int scale;
    int taskSide;

    for (side = 0; side < 2; side++) {
        list = &MENU->battle.unk_2ec[side];
        while (list->cursor != list->count && screen->frame == list->key[list->cursor]) {
            if (MENU->battle.unk_27c[side]->unk_48 == 0) {
                if (!DUE.unk_6) {
                    if (MENU->battle.unk_58[side] != NULL) {
                        task = (BattleCellTask *)MENU->battle.unk_310[side][DUE.cell].unk_00;
                        taskSide = task->side;
                        scale = task->scale * 384 >> 8;
                        MENU->battle.unk_22c[side][DUE.unk_3] = (BattleCounterTask *)BattleSpriteTask_Create(
                            MENU->battle.unk_58[task->side], 0, task->posX, task->posY, task->posZ,
                            BattleCellTask_GetCellOffsetY(task, DUE.unk_3),
                            task->angle + BattleCellTask_GetCellOffsetX(task, DUE.unk_3), -1, scale, scale,
                            (&MENU->colours[12])[task->side], 8, 0, taskSide, task->hasPos2,
                            task->pos2X, task->pos2Y, task->pos2Z, 8);
                    }
                } else {
                    task = (BattleCellTask *)MENU->battle.unk_310[side][DUE.cell].unk_00;
                    task->steps[task->stepIndex] = 5;
                    task->stepIndex++;
                    if (task->stepIndex == 8) {
                        task->stepIndex = 0;
                    }
                }
            } else {
                if (!DUE.unk_6) {
                    if (MENU->battle.unk_58[side] != NULL) {
                        task = (BattleCellTask *)MENU->battle.unk_310[side][0].unk_00;
                        taskSide = task->side;
                        scale = task->scale * 384 >> 8;
                        MENU->battle.unk_22c[side][DUE.unk_3] = (BattleCounterTask *)BattleSpriteTask_Create(
                            MENU->battle.unk_58[task->side], 0, task->posX, task->posY, task->posZ,
                            BattleCellTask_GetCellOffsetY(task, DUE.unk_3),
                            task->angle + BattleCellTask_GetCellOffsetX(task, DUE.unk_3), -1, scale, scale,
                            (&MENU->colours[12])[task->side], 8, 0, taskSide, task->hasPos2,
                            task->pos2X, task->pos2Y, task->pos2Z, 8);
                    }
                } else {
                    task = (BattleCellTask *)MENU->battle.unk_310[side][0].unk_00;
                    task->steps[task->stepIndex] = 5;
                    task->stepIndex++;
                    if (task->stepIndex == 8) {
                        task->stepIndex = 0;
                    }
                }
            }
            if (MENU->battle.unk_58[side] != NULL) {
                Battle_StartSideShake(side);
            }
            list->cursor++;
        }
    }
}
