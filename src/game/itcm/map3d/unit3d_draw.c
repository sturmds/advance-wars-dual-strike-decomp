#include "types.h"
#include "sdk/fx.h"

int Unit3d_GetTextureOffset(int player, int unitType);
void Map3D_DrawUnitSprite(VecFx32 *pos, int texOffset, int palette, int shaded, int arg4, int arg5);

/* Draws a unit's image: Map3D_DrawUnitSprite with the texture offset of the unit
 * type for the player's army style. */
void Unit3d_Draw(VecFx32 *pos, int player, int unitType, int palette, int shaded, int arg5, int arg6)
{
    Map3D_DrawUnitSprite(pos, Unit3d_GetTextureOffset(player, unitType), palette, shaded, arg5, arg6);
}
