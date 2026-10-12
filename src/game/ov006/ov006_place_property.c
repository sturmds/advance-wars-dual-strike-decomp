#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/Unk_ov006_0235f690.h"

int Ov006_GetTilePropertyKind(
    int x, int y); /* 2: HQ, 1: other property, 0: no property */
void Ov006_RemoveProperty(int x, int y);
void Ov006_AddProperty(int x, int y, int tile);
void Ov006_ResetNeighborsToSea(int x, int y);
void Ov006_CheckReefsAround(int x, int y);
void Ov006_BreakCellBlock(int x, int y);
BOOL Ov006_GetHqPosition(int hqTile, int* x,
                         int* y); /* the army's HQ position, if it has one */
void Ov006_ClearHqPosition(
    int hqTile); /* sets that army's hqX and hqY back to -1 */
void Ov006_SetHqPosition(int hqTile, int x,
                         int y); /* stores x, y as that army's HQ position */
int Ov006_GetPropertyCell(int tile); /* cell code for a tile byte (guess) */
void Ov006_SetTerrain(int x, int y, int terrain);
void func_ov006_0235da70(int x, int y, int value);
void Ov006_RefitCellsAround(int x, int y);
void Ov006_RefreshNeighborRiverCells(int x, int y);
void func_020c4448(void);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_AT(x, y) \
  (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

#define PROPERTY_NONE 0
#define PROPERTY_HQ 2
#define MAX_PROPERTIES 60 /* HQs do not count against it */
#define CELL_PLAIN 1

/* Puts the property tile (terrain and owner) on x, y of the map being
 * edited. An army has one HQ: placing one turns the army's old HQ into plain.
 * Returns -1 when the map already has MAX_PROPERTIES properties, 0 when the
 * tile is already there, 1 when it was placed. */
int Ov006_PlaceProperty(int x, int y, int tile) {
  int hqX;
  int hqY;
  u16 cell;

  if (MAP_TILE_TERRAIN(tile) != TERRAIN_HQ &&
      data_ov006_0236d640->propertyCount >= MAX_PROPERTIES) {
    if (Ov006_GetTilePropertyKind(x, y) == PROPERTY_NONE) {
      return -1;
    }
  }
  if (Ov006_GetTilePropertyKind(x, y) == PROPERTY_HQ) {
    if (tile == TILE_AT(x, y)) {
      return 0;
    }
  } else if (Ov006_GetTilePropertyKind(x, y) == PROPERTY_NONE &&
             data_ov006_0236d640->propertyCount >= MAX_PROPERTIES) {
    return -1;
  }
  if (Ov006_GetHqPosition(tile, &hqX, &hqY)) {
    Ov006_ClearHqPosition(tile);
    Ov006_RemoveProperty(hqX, hqY);
    Ov006_SetTerrain(hqX, hqY, TERRAIN_PLAIN);
    func_ov006_0235da70(hqX, hqY, CELL_PLAIN);
  }
  if (Ov006_GetTilePropertyKind(x, y) == PROPERTY_HQ) {
    Ov006_ClearHqPosition(TILE_AT(x, y));
  }
  cell = Ov006_GetPropertyCell(tile);
  if (Ov006_GetTilePropertyKind(x, y) != PROPERTY_NONE) {
    if (tile == TILE_AT(x, y)) {
      return 0;
    }
    Ov006_RemoveProperty(x, y);
  }
  Ov006_SetTerrain(x, y, tile);
  func_ov006_0235da70(x, y, cell);
  Ov006_AddProperty(x, y, tile);
  func_020c4448();
  Ov006_SetHqPosition(tile, x, y);
  Ov006_RefreshNeighborRiverCells(x, y);
  Ov006_ResetNeighborsToSea(x, y);
  Ov006_RefitCellsAround(x, y);
  Ov006_CheckReefsAround(x, y);
  Ov006_BreakCellBlock(x, y);
  return 1;
}
