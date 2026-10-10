#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSpriteTask.h"

extern MenuState gMenuState;

#define MENU ((MenuState *)&gMenuState)

BattleSpriteTask *func_ov002_02356604(int table, int index, int x, int y, int unk_40, int offsetX, int offsetY, int unk_4c, int scaleX, int scaleY, int unk_54, int unk_56, int unk_59, int side);

/* Needed to match: the point of the cell in the second set, with the offset 0x26 of the table
 * kept in the load. */
typedef struct PointAt {
    u8 pad_00[0x26];
    BattlePoint point;
} PointAt;


#define SPRITE (MENU->battle.cellSprites[side][cell])
#define OTHER_EFFECT (MENU->battle.unk_27c[other]->effects[Ov002_gBattleScene->sides[other].usesSecondary])

void func_ov002_02357488(int side, int cell, int param)
{
    BattleSpriteTask *cellTask = (BattleSpriteTask *)MENU->battle.unk_310[side][0].unk_00;
    BattleSideData *sideData = MENU->battle.unk_27c[side];
    int other = (side + 1) & 1;
    u8 taskSide;
    int taskOther;
    int unk_4c = -1;
    int unk_56 = 8;
    PointAt *point = (PointAt *)((u8 *)sideData->cellPositions + cell * sizeof(BattlePoint));

    SPRITE.pos.x = cellTask->unk_38;
    SPRITE.pos.y = cellTask->unk_3a;
    SPRITE.step.x = point->point.x + OTHER_EFFECT->unk_26.x;
    SPRITE.step.y = cellTask->unk_46 + (point->point.y + OTHER_EFFECT->unk_26.y);
    SPRITE.target.x = cellTask->unk_38 + point->point.x;
    SPRITE.target.y = cellTask->unk_3a + point->point.y + cellTask->unk_46;
    SPRITE.cell = 0;
    SPRITE.unk_14_3 = 0;
    SPRITE.unk_14_hi = (u8)param;
    taskSide = cellTask->side;
    taskOther = (taskSide + 1) & 1;
    SPRITE.task = func_ov002_02356604(
        MENU->battle.unk_48[taskOther], 1, MENU->battle.cellSprites[taskSide][cell].pos.x, MENU->battle.cellSprites[taskSide][cell].pos.y,
        cellTask->unk_3c, MENU->battle.cellSprites[taskSide][cell].step.x, MENU->battle.cellSprites[taskSide][cell].step.y, unk_4c,
        cellTask->scale, cellTask->scale, (&MENU->colours[8])[taskOther], unk_56, 1, taskSide);
    SPRITE.direction.x = OTHER_EFFECT->direction.x;
    SPRITE.direction.y = OTHER_EFFECT->direction.y;
}
