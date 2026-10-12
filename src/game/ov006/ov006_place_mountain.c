#include "types.h"
#include "game/terrain.h"

int Ov006_IsLandTile(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
void func_ov006_02363198(int x, int y);
void Ov006_RefreshNeighborRiverCells(int x, int y);
void Ov006_ResetNeighborsToSea(int x, int y);
void Ov006_RefitCellsAround(int x, int y);
void Ov006_CheckReefsAround(int x, int y);
void Ov006_BreakCellBlock(int x, int y);

/* Puts a mountain at (x, y), going through plain first when
 * Ov006_IsLandTile returns 0, then runs the follow-up fixes for the tile. */
void Ov006_PlaceMountain(int x, int y) {
  if (!Ov006_IsLandTile(x, y)) {
    Ov006_SetTerrain(x, y, TERRAIN_PLAIN);
  }
  Ov006_SetTerrain(x, y, TERRAIN_MOUNTAIN);
  func_ov006_02363198(x, y);
  Ov006_RefreshNeighborRiverCells(x, y);
  Ov006_ResetNeighborsToSea(x, y);
  Ov006_RefitCellsAround(x, y);
  Ov006_CheckReefsAround(x, y);
  Ov006_BreakCellBlock(x, y);
}
