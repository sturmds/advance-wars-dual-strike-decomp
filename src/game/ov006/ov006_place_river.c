#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_GetNeighborTile(int x, int y);
void Ov006_RefreshNeighborRiverCells(int x, int y);
int Ov006_GetRiverPiece(int x, int y);
int Ov006_GetLandMaskPiece(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);
int Ov006_TileMatches(int x, int y, int arg);
int Ov006_CountRiverNeighbors(int x, int y);
void Ov006_AddRiverMouths(int x, int y);
void func_ov006_0235dd24(int x, int y);
void Ov006_PlaceMountain(int x, int y);
void func_ov006_0235da70(int x, int y, int arg);
void Ov006_RefitCellsAround(int x, int y);
void Ov006_BreakCellBlock(int x, int y);

/* Puts a river on tile x, y. The cell code comes from the 3x3 block around
 * it (Ov006_GetNeighborTile), or from Ov006_GetRiverPiece or
 * Ov006_GetLandMaskPiece when that gives 0. After the neighbours are fixed up,
 * the tiles above and below get their own updates. Returns 0 when no river
 * fits there, 1 when one was placed. */
int Ov006_PlaceRiver(int x, int y) {
  int blockCell;
  int cell;

  blockCell = Ov006_GetNeighborTile(x, y);
  if (blockCell < 0) {
    return 0;
  }
  if (blockCell != 0) {
    cell = Ov006_GetRiverPiece(x, y);
    if (cell == 0) {
      Ov006_SetTerrain(x, y, TERRAIN_RIVER);
      Ov006_StoreMapCell(x, y, blockCell);
      Ov006_RefreshNeighborRiverCells(x, y);
    } else if (cell > 0) {
      blockCell &= 0xfe00;
      /* needed to match: each flag value is tested twice */
      if (blockCell == 0x4000 && blockCell == 0x4000) {
        Ov006_SetTerrain(x, y, TERRAIN_RIVER);
        Ov006_StoreMapCell(x, y, cell);
      }
      if (blockCell == 0x2000 && blockCell == 0x2000) {
        Ov006_SetTerrain(x, y, TERRAIN_RIVER);
        Ov006_StoreMapCell(x, y, cell);
      }
      if (blockCell == 0) {
        Ov006_SetTerrain(x, y, TERRAIN_RIVER);
        Ov006_StoreMapCell(x, y, cell);
      }
      Ov006_RefreshNeighborRiverCells(x, y);
    } else {
      return 0;
    }
  } else if (Ov006_GetNeighborTile(x, y) >= 0) {
    cell = Ov006_GetLandMaskPiece(x, y);
    if (cell != 0) {
      blockCell = Ov006_GetRiverPiece(x, y);
      if (blockCell != 0) {
        cell = blockCell;
      }
    }
    if (cell > 0) {
      Ov006_SetTerrain(x, y, TERRAIN_RIVER);
      Ov006_StoreMapCell(x, y, cell);
      Ov006_RefreshNeighborRiverCells(x, y);
    } else {
      return 0;
    }
  } else {
    return 0;
  }

  if (Ov006_CountRiverNeighbors(x, y) != 0) {
    Ov006_AddRiverMouths(x, y);
  }
  if (y > 0 && Ov006_TileMatches(x, y - 1, 1) != 0) {
    Ov006_StoreMapCell(x, y - 1, 1);
    func_ov006_0235dd24(x, y - 1);
    if (MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] ==
        TERRAIN_MOUNTAIN) {
      Ov006_PlaceMountain(x, y);
    }
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1 &&
      Ov006_TileMatches(x, y + 1, 1) != 0) {
    func_ov006_0235da70(x, y + 1, 1);
  }
  Ov006_RefitCellsAround(x, y);
  Ov006_BreakCellBlock(x, y);
  return 1;
}
