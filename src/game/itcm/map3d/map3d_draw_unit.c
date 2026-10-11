#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Map3d_DrawUnitSprite with the sprite of unitType in player's army style. */
void Unit3d_Draw(VecFx32 *pos, int player, int unitType, int palette, int flip, int shaded, int ghost);
int KeyIcon_GetTextureOffset(int flags);          /* status icon tile for flags, animated; -1: none */
int Digit3d_GetTextureOffset(int hp, int flags);  /* hp digit tile, or the UNIT_DRAW_SKILL icon */
void Map3d_DrawIcon(VecFx32 *pos, int tile, int palette, int hidden);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* flags bits this function reads; the caller sets more for KeyIcon_GetTextureOffset */
#define UNIT_DRAW_SKILL  0x001 /* the caller sets it from CheckSkillsEquipped; an icon replaces the hp digit */
#define UNIT_DRAW_WAITED 0x100 /* MAP_UNIT_WAITED: draw in the army's waited palette */

/* Draws a unit on the 3D map at pos: its sprite, a status icon 4 to the left
 * and its hp digit (1-9) 4 to the right. ownerBase holds the owner's first unit
 * slot (0, 64, 128 or 192); the units of players 1 and 3 face the other way.
 * ghost draws the sprite translucent and leaves out the icons. */
void Map3d_DrawUnit(VecFx32 *pos, int unitType, int ownerBase, int hp, int flags, int shaded, int ghost)
{
    VecFx32 at = *pos;
    int palette;

    if (flags & UNIT_DRAW_WAITED) {
        palette = STATE->players[(ownerBase >> 6) + 1].armyColour + 4;
    } else {
        palette = STATE->players[(ownerBase >> 6) + 1].armyColour - 1;
    }
    Unit3d_Draw(&at, (ownerBase >> 6) + 1, unitType, palette, ownerBase == 0 || ownerBase == 128, shaded, ghost);
    at.x -= 4;
    Map3d_DrawIcon(&at, KeyIcon_GetTextureOffset(flags), palette, ghost);
    if (!(flags & UNIT_DRAW_SKILL)) {
        if (hp < 1) {
            return;
        }
        if (hp >= 10) {
            return;
        }
    }
    at.x += 8;
    Map3d_DrawIcon(&at, Digit3d_GetTextureOffset(hp, flags), palette, ghost);
}
