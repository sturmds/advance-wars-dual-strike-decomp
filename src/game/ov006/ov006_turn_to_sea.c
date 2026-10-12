#include "types.h"
#include "game/terrain.h"

int Ov006_TileMatches(
    int x, int y,
    int terrain); /* 1 when tile x, y is on the map and has this terrain byte */
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);
int Ov006_GetTilePropertyKind(int x, int y);
void Ov006_RemoveProperty(int x, int y);
int Ov006_IsRiverOrBridgeOnRiver(int x, int y);
int Ov006_IsSpecialCell(int x, int y);
void func_ov006_0235da70(int x, int y, int cell);
void Ov006_FitWaterCellsInBlock(int x, int y);
void Ov006_RefreshNeighborRiverCells(int x, int y);
void Ov006_ResetNeighborsToSea(int x, int y);
void Ov006_RefitCellsAround(int x, int y);
void Ov006_BreakCellBlock(int x, int y);

#define CELL_PLAIN 1
#define CELL_SEA 42 /* the cell Ov006_FillMapForScreen uses for sea */

/* Turns tile x, y into sea, unless it already is. A property on it is
 * removed first; a river or a bridge on a river is cleared to plain (or set to
 * the sea cell for Ov006_IsSpecialCell cells). A reef only changes terrain and
 * cell. Any other tile then refits the water cells of its 3x3 block and the
 * cells around it, and breaks the cell block it was part of. */
void Ov006_TurnToSea(int x, int y) {
  if (Ov006_TileMatches(x, y, TERRAIN_SEA)) {
    return;
  }
  if (Ov006_GetTilePropertyKind(x, y)) {
    Ov006_RemoveProperty(x, y);
  }
  if (Ov006_IsRiverOrBridgeOnRiver(x, y)) {
    if (Ov006_IsSpecialCell(x, y)) {
      Ov006_StoreMapCell(x, y, CELL_SEA);
    } else {
      func_ov006_0235da70(x, y, CELL_PLAIN);
    }
  }
  if (Ov006_TileMatches(x, y, TERRAIN_REEF)) {
    Ov006_SetTerrain(x, y, TERRAIN_SEA);
    Ov006_StoreMapCell(x, y, CELL_SEA);
    return;
  }
  Ov006_SetTerrain(x, y, TERRAIN_SEA);
  Ov006_FitWaterCellsInBlock(x, y);
  Ov006_RefreshNeighborRiverCells(x, y);
  Ov006_ResetNeighborsToSea(x, y);
  Ov006_RefitCellsAround(x, y);
  Ov006_BreakCellBlock(x, y);
}
