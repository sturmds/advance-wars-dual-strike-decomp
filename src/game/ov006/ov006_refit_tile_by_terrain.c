#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_TileMatches(
    int x, int y,
    int terrain); /* 1 when tile x, y is on the map and has this terrain byte */
void Ov006_StoreMapCell(int x, int y, int cell);
int Ov006_GetCellMatchedToLeft(int x, int y);
int Ov006_GetRoadCell(int x, int y);
void func_ov006_02363198(int x, int y);
s16 Ov006_GetShoalGraphic(int x, int y);
void Ov006_TurnToSea(int x, int y);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Picks the cell of tile x, y again from its terrain: plain, mountain, road
 * and shoal tiles. Plain cells 67 and 3 go back to cell 1 when the tile below
 * is no longer a mountain (guess: they draw the top of that mountain). A
 * shoal that no shoal graphic fits becomes sea (Ov006_TurnToSea). */
void Ov006_RefitTileByTerrain(int x, int y) {
  int cell;

  if (Ov006_TileMatches(x, y, TERRAIN_PLAIN)) {
    cell = STATE->mapTiles->mapCells[x + STATE->mapTiles->rowStart[y]];
    if (cell == 67 || cell == 3) {
      if (!Ov006_TileMatches(x, y + 1, TERRAIN_MOUNTAIN)) {
        Ov006_StoreMapCell(x, y, 1);
      }
    }
    Ov006_StoreMapCell(x, y, Ov006_GetCellMatchedToLeft(x, y));
  }
  if (Ov006_TileMatches(x, y, TERRAIN_MOUNTAIN)) {
    func_ov006_02363198(x, y);
  }
  if (Ov006_TileMatches(x, y, TERRAIN_ROAD)) {
    Ov006_StoreMapCell(x, y, Ov006_GetRoadCell(x, y));
    Ov006_StoreMapCell(x, y, Ov006_GetCellMatchedToLeft(x, y));
  }
  if (Ov006_TileMatches(x, y, TERRAIN_SHOAL)) {
    cell = Ov006_GetShoalGraphic(x, y);
    if (cell < 0) {
      Ov006_TurnToSea(x, y);
    } else {
      Ov006_StoreMapCell(x, y, cell);
    }
  }
}
