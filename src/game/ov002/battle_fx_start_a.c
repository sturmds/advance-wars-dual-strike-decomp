#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSpriteTask.h"

extern MenuState gMenuState;

#define MENU ((MenuState *)&gMenuState)

/* Needed to match: the point of the cell, with the offset 0xe of the table kept in the load. */
typedef struct PointAt {
    u8 pad_00[0x0e];
    BattlePoint point;
} PointAt;

BattleSpriteTask *func_ov002_02356604(int table, int index, int x, int y, int unk_40, int offsetX, int offsetY, int unk_4c, int scaleX, int scaleY, int unk_54, int unk_56, int unk_59, int side);

void func_ov002_02357e5c(int side, int cell)
{
    BattleSpriteTask *cellTask = (BattleSpriteTask *)MENU->battle.unk_310[side][0].unk_00;
    BattleEffectParams *effect;
    int taskSide;
    PointAt *point = (PointAt *)((u8 *)MENU->battle.unk_27c[side]->cellPositions + cell * sizeof(BattlePoint));

    MENU->battle.motions[side][cell].x = cellTask->unk_38;
    MENU->battle.motions[side][cell].y = cellTask->unk_3a;
    effect = MENU->battle.unk_27c[side]->effects[Ov002_gBattleScene->sides[side].usesSecondary];
    MENU->battle.motions[side][cell].speedX = (cellTask->scale * (point->point.x + effect->offset.x)) >> 8;
    effect = MENU->battle.unk_27c[side]->effects[Ov002_gBattleScene->sides[side].usesSecondary];
    MENU->battle.motions[side][cell].speedY = cellTask->unk_46 + ((cellTask->scale * (point->point.y + effect->offset.y)) >> 8);
    taskSide = cellTask->side;
    MENU->battle.motions[side][cell].unk_00 = (s32)func_ov002_02356604(
        MENU->battle.unk_48[taskSide], 0, MENU->battle.motions[taskSide][cell].x, MENU->battle.motions[taskSide][cell].y,
        cellTask->unk_3c, MENU->battle.motions[taskSide][cell].speedX, MENU->battle.motions[taskSide][cell].speedY, -1,
        cellTask->scale, cellTask->scale, (&MENU->colours[8])[taskSide], 8, 0, taskSide);
    effect = MENU->battle.unk_27c[side]->effects[Ov002_gBattleScene->sides[side].usesSecondary];
    MENU->battle.motions[side][cell].velocityX = (cellTask->scale * effect->velocity.x) >> 8;
    effect = MENU->battle.unk_27c[side]->effects[Ov002_gBattleScene->sides[side].usesSecondary];
    MENU->battle.motions[side][cell].velocityY = (cellTask->scale * effect->velocity.y) >> 8;
    effect = MENU->battle.unk_27c[side]->effects[Ov002_gBattleScene->sides[side].usesSecondary];
    MENU->battle.motions[side][cell].delay = effect->delay;
}
