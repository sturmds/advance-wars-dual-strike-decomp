#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSpriteTask.h"

extern MenuState gMenuState;

#define MENU ((MenuState *)&gMenuState)

#define SPRITE (MENU->battle.cellSprites[side][cell])

/* Needed to match: the side's record is reached as a byte offset of the scene. */
#define SIDE_WEAPON(side) (((u8 *)Ov002_gBattleScene + (side) * sizeof(BattleSide))[9])

BOOL func_ov002_02357d24(int side, int cell)
{
    int reached = 0;
    int axis;
    int next;
    int target;
    int direction;
    int other = (side + 1) & 1;
    BattleSideData *sideData = MENU->battle.unk_27c[other];

    for (axis = 0; axis < 2; axis++) {
        switch (axis) {
        case 0:
            next = SPRITE.pos.x + SPRITE.step.x;
            target = SPRITE.target.x;
            direction = sideData->effects[SIDE_WEAPON(other)]->unk_2e.x;
            if (direction == 0) {
                direction = SPRITE.direction.x;
            }
            break;
        case 1:
            next = SPRITE.pos.y + SPRITE.step.y;
            target = SPRITE.target.y;
            direction = sideData->effects[SIDE_WEAPON(other)]->unk_2e.y;
            if (direction == 0) {
                direction = SPRITE.direction.y;
            }
            break;
        }
        if (direction == 0) {
            direction = 0;
        } else if (direction > 0) {
            direction = 1;
        } else {
            direction = 2;
        }
        switch (direction) {
        case 0:
            reached++;
            break;
        case 1:
            if (next >= target) {
                reached++;
            }
            break;
        case 2:
            if (next <= target) {
                reached++;
            }
            break;
        }
    }
    return reached == 2;
}
