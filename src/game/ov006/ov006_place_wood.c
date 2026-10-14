#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

int Ov006_GetTilePropertyKind(
    int x, int y); /* 2: HQ, 1: other property, 0: no property */
void Ov006_RemoveProperty(int x, int y);
void Ov006_ResetNeighborsToSea(int x, int y);
void Ov006_CheckReefsAround(int x, int y);
void Ov006_RefitCellToRight(
    int x, int y); /* guess: refreshes the cell to the right of x, y */
void Ov006_SetTerrain(int x, int y, int terrain);
void func_ov006_0235da70(int x, int y, int value);
void func_ov006_0235dd24(int x, int y);
void Ov006_RefitCellsAround(int x, int y);
void Ov006_RefreshNeighborRiverCells(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define PROPERTY_NONE 0

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

/* Puts a wood on x, y of the map being edited, removing a property first.
 * A wood tile is redrawn only when its cell is 134 or MAP_CELL_WOOD; other
 * wood cells are kept. Then runs the follow-up fixes for the tile. */
void Ov006_PlaceWood(int x, int y) {
  MapTiles* tiles;
  int i;

  if (Ov006_GetTilePropertyKind(x, y) != PROPERTY_NONE) {
    Ov006_RemoveProperty(x, y);
    Ov006_SetTerrain(x, y, TERRAIN_PLAIN);
  }
  tiles = STATE->mapTiles;
  i = x + tiles->rowStart[y];
  if (tiles->mapCells[i] == MAP_CELL_WOOD ||
      tiles->mapCells[i] == 134 /* another wood cell */ ||
      tiles->tiles[i] != TERRAIN_WOOD) {
    Ov006_RefitCellToRight(x, y);
    Ov006_SetTerrain(x, y, TERRAIN_WOOD);
    func_ov006_0235da70(x, y, MAP_CELL_WOOD);
    func_ov006_0235dd24(x, y);
  }
  Ov006_RefreshNeighborRiverCells(x, y);
  Ov006_ResetNeighborsToSea(x, y);
  Ov006_RefitCellsAround(x, y);
  Ov006_CheckReefsAround(x, y);
}
