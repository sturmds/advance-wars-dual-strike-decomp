#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

int Ov006_IsBareSea(int x, int y);
int Ov006_GetTilePropertyKind(int x, int y);
void Ov006_RemoveProperty(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);
void Ov006_BreakCellBlock(int x, int y);
void Ov006_PaintSea(int x, int y);
void Ov006_RefitAround(int x, int y);

#define MAP_INFO (MAP_STATE->unk_1c4)

/* Puts reef on the sea tile at x, y. 0: refused (not bare sea); 1: placed.
 * Afterwards each of the eight neighbours inside the map goes through
 * Ov006_PaintSea, then all of them again through Ov006_RefitAround. */
int Ov006_PlaceReef(int x, int y) {
  if (!Ov006_IsBareSea(x, y)) {
    return 0;
  }
  if (Ov006_GetTilePropertyKind(x, y)) {
    Ov006_RemoveProperty(x, y);
  }
  data_ov006_0236d640->oldCell =
      MAP_STATE->mapTiles->mapCells[x + MAP_STATE->mapTiles->rowStart[y]];
  Ov006_SetTerrain(x, y, TERRAIN_REEF);
  Ov006_StoreMapCell(x, y, MAP_CELL_REEF);
  Ov006_BreakCellBlock(x, y);

  if (y > 0) {
    if (x > 0) {
      Ov006_PaintSea(x - 1, y - 1);
    }
    Ov006_PaintSea(x, y - 1);
    if (x < MAP_INFO->mapWidth - 1) {
      Ov006_PaintSea(x + 1, y - 1);
    }
  }
  if (x > 0) {
    Ov006_PaintSea(x - 1, y);
  }
  if (x < MAP_INFO->mapWidth - 1) {
    Ov006_PaintSea(x + 1, y);
  }
  if (y < MAP_INFO->mapHeight - 1) {
    if (x > 0) {
      Ov006_PaintSea(x - 1, y + 1);
    }
    Ov006_PaintSea(x, y + 1);
    if (x < MAP_INFO->mapWidth - 1) {
      Ov006_PaintSea(x + 1, y + 1);
    }
  }

  if (y > 0) {
    if (x > 0) {
      Ov006_RefitAround(x - 1, y - 1);
    }
    Ov006_RefitAround(x, y - 1);
    if (x < MAP_INFO->mapWidth - 1) {
      Ov006_RefitAround(x + 1, y - 1);
    }
  }
  if (x > 0) {
    Ov006_RefitAround(x - 1, y);
  }
  if (x < MAP_INFO->mapWidth - 1) {
    Ov006_RefitAround(x + 1, y);
  }
  if (y < MAP_INFO->mapHeight - 1) {
    if (x > 0) {
      Ov006_RefitAround(x - 1, y + 1);
    }
    Ov006_RefitAround(x, y + 1);
    if (x < MAP_INFO->mapWidth - 1) {
      Ov006_RefitAround(x + 1, y + 1);
    }
  }

  Ov006_SetTerrain(x, y, TERRAIN_REEF);
  Ov006_StoreMapCell(x, y, MAP_CELL_REEF);
  return 1;
}
