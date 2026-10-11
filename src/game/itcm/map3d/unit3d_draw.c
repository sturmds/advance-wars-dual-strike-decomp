#include "types.h"
#include "sdk/fx.h"

int Unit3d_GetTextureOffset(int player, int unitType);
void func_01ffbf20(VecFx32 *pos, int texOffset, int palette, int shaded, int arg4, int arg5);

/* Draws a unit's image: func_01ffbf20 with the texture offset of the unit
 * type for the player's army style. */
void Unit3d_Draw(VecFx32 *pos, int player, int unitType, int palette, int shaded, int arg5, int arg6)
{
    func_01ffbf20(pos, Unit3d_GetTextureOffset(player, unitType), palette, shaded, arg5, arg6);
}
