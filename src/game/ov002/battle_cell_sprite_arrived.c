#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/BattleSpriteTask.h"

extern MenuState gMenuState;

#define MENU ((MenuState *)&gMenuState)

#define SPRITE (MENU->battle.cellSprites[side][cell])

/* Needed to match: the side's record is reached as a byte offset of the scene. */
#define SIDE_WEAPON(side) (((u8 *)Ov002_gBattleScene + (side) * sizeof(BattleSide))[9])

/* TRUE when the cell sprite's next step reaches its target on both axes. The heading on an
 * axis comes from the other side's effect, or from the sprite when the effect gives none;
 * an axis with no heading counts as reached. */
BOOL BattleCellSprite_Arrived(int side, int cell)
{
    int direction;
    int reached = 0;
    int next;
    int target;
    int axis;
    int other = (side + 1) & 1;

    for (axis = 0; axis < 2; axis++) {
        switch (axis) {
        case 0:
            direction = MENU->battle.unk_27c[other]->effects[SIDE_WEAPON(other)]->unk_2e.x;
            if (direction == 0) {
                direction = SPRITE.direction.x;
            }
            next = SPRITE.pos.x + SPRITE.step.x;
            target = SPRITE.target.x;
            break;
        case 1:
            direction = MENU->battle.unk_27c[other]->effects[SIDE_WEAPON(other)]->unk_2e.y;
            if (direction == 0) {
                direction = SPRITE.direction.y;
            }
            next = SPRITE.pos.y + SPRITE.step.y;
            target = SPRITE.target.y;
            break;
        }
        /* 0: no heading, 1: increasing, 2: decreasing */
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
