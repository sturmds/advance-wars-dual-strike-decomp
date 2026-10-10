#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

#define MENU ((MenuState *)&gMenuState)

/* Needed to match: the original adds side * 120 + cell * 24 to the start of the
 * menu state and reaches the sprite from there. */
typedef struct DirAt {
    u8 pad_000[0x148];
    BattlePoint direction;
} DirAt;

typedef struct CellSpriteAt {
    u8 pad_000[0x140];
    BattlePoint pos;
    BattlePoint step;
    BattlePoint direction;
    BattlePoint target;
} CellSpriteAt;

/* Needed to match: the side's record is reached as a byte offset of the scene. */
#define SIDE_WEAPON(side) (((u8 *)Ov002_gBattleScene + (side) * sizeof(BattleSide))[9])

BOOL func_ov002_02357d24(int side, int cell)
{
    int direction;
    int reached;
    int next;
    int target;
    BattleSideData *sideData;
    CellSpriteAt *sprite;
    int axis;
    int other;
    DirAt *dirs;
    other = (side + 1) & 1;
    sprite = ((CellSpriteAt *)((u8 *)MENU + cell * sizeof(BattleCellSprite) + side * sizeof(MENU->battle.unk_13c[0])));
    dirs = (DirAt *)sprite;
    sideData = MENU->battle.unk_27c[other];
    reached = 0;

    for (axis = 0; axis < 2; axis++) {
        switch (axis) {
        case 0:
            next = sprite->pos.x + sprite->step.x;
            target = sprite->target.x;
            direction = sideData->effects[SIDE_WEAPON(other)]->unk_2e.x;
            if (direction == 0) {
                direction = dirs->direction.x;
            }
            break;
        case 1:
            next = sprite->pos.y + sprite->step.y;
            target = sprite->target.y;
            direction = sideData->effects[SIDE_WEAPON(other)]->unk_2e.y;
            if (direction == 0) {
                direction = dirs->direction.y;
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
