#include "types.h"
#include "game/battle_scene.h"
#include "game/math.h"

void Bg_PutTileRect(u16* map, u16 x, u16 y, int width, u16 height,
                    int firstTile, u16 palette);

/* Draws the side's row of cells in the tile map: a 4-tile strip at its edge, then one
 * tile for each ten of BattleSide.terrainDefense. Needed to match: ground is assigned inside
 * the first call's arguments. */
void Battle_PutSideCells(u16* tilemap, int side, int isAir) {
  int i;
  int count = HwDiv32(Ov002_gBattleScene->sides[side].terrainDefense, 10);
  BOOL left = (side == 0);
  BOOL ground;

  Bg_PutTileRect(tilemap, left ? 0 : 28, (ground = (isAir == 0)) ? 6 : 17, 4, 1,
                 25, left ? 8 : 9);
  for (i = 0; i < count; i++) {
    Bg_PutTileRect(tilemap, left ? i + 4 : 28 - (i + 1), ground ? 6 : 17, 1, 1,
                   29, left ? 8 : 9);
  }
}
