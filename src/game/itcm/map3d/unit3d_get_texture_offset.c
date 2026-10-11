#include "types.h"

int Player_GetArmyStyle(int player);

/* ITCM table of 27 words per army style, indexed by unit type. */
extern u32 gUnit3dTextureBlocks[][27];

/* Texture image offset of a unit type for the player's army style. */
int Unit3d_GetTextureOffset(int player, int unitType)
{
    return gUnit3dTextureBlocks[Player_GetArmyStyle(player)][unitType] << 9;
}
