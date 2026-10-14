#include "types.h"
#include "game/battle_scene.h"

BOOL Battle_IsSideAir(int side);
void Battle_PutSideBlock490(void* tilemap, u8 side, u8 isAir);
void Battle_PutSideHpBlock(void* tilemap, u8 side, u8 isAir);
void Battle_PutSideCells(void* tilemap, u8 side, u8 isAir);
void Battle_PutSideBlock750(void* tilemap, u8 side, u8 isAir);

/* Writes the tile maps of both sides into tilemap in four steps (callees not
 * named yet, at least one calls Bg_PutTileBlock). isAir is passed to each. */
void Battle_BuildSideTilemaps(void* tilemap) {
  int side;
  BOOL isAir;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    isAir = Battle_IsSideAir(side) ? TRUE : FALSE;
    Battle_PutSideBlock490(tilemap, side, isAir);
    Battle_PutSideHpBlock(tilemap, side, isAir);
    Battle_PutSideCells(tilemap, side, isAir);
    Battle_PutSideBlock750(tilemap, side, isAir);
  }
}
