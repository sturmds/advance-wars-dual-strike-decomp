#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

void Ov006_SetMapCell(int x, int y, int cell);
int Ov006_PlaceShoal(int x, int y);
void Ov006_TurnToSea(int x, int y); /* refits a sea tile */
int Ov006_PlaceReef(int x, int y);  /* places reef; 0: refused */
void Ov006_PlaceWood(int x, int y); /* places wood */
void Ov006_PlaceRoad(int x, int y); /* places road */
void Ov006_PlacePipe(int x, int y); /* places pipe */

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern u8 Ov006_gPresetMapTiles
    []; /* one tile byte (terrain and owner) per tile, by row */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[STATE->mapTiles->rowStart[y] + (x)])
#define TILE_AT(x, y) \
  (STATE->mapTiles->tiles[STATE->mapTiles->rowStart[y] + (x)])

/* Fills the Design Room map with a built-in map, which
 * Ov006_NewMap loads instead of its usual one when L and R are both
 * held. The map starts as sea; then each tile of Ov006_gPresetMapTiles gets its
 * cell code through the placing routine for its terrain. Shoal goes last, once
 * the land around it is in place. */
void Ov006_LoadPresetMap(void) {
  int x;
  int y;
  u8* src;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      CELL_AT(x, y) = MAP_CELL_SEA;
      TILE_AT(x, y) = TERRAIN_SEA;
    }
  }

  src = Ov006_gPresetMapTiles;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      switch (*src++) {
        case TERRAIN_PLAIN:
          CELL_AT(x, y) = 1;
          TILE_AT(x, y) = TERRAIN_PLAIN;
          break;
        case TERRAIN_MOUNTAIN:
          CELL_AT(x, y) = 32;
          TILE_AT(x, y) = TERRAIN_MOUNTAIN;
          break;
        case TERRAIN_WOOD:
          CELL_AT(x, y) = 135;
          if (TILE_AT(x, y) != TERRAIN_WOOD) {
            TILE_AT(x, y) = TERRAIN_WOOD;
            CELL_AT(x, y) = 135;
            Ov006_PlaceWood(x, y);
          }
          break;
        case TERRAIN_SEA:
          CELL_AT(x, y) = MAP_CELL_SEA;
          Ov006_TurnToSea(x, y);
          break;
        case TERRAIN_REEF:
          CELL_AT(x, y) = 360;
          Ov006_PlaceReef(x, y);
          break;
        case TERRAIN_ROAD:
          CELL_AT(x, y) = 97;
          Ov006_PlaceRoad(x, y);
          break;
        case TERRAIN_PIPE:
        case TERRAIN_PIPE_SEAM:
          CELL_AT(x, y) = 322;
          Ov006_PlacePipe(x, y);
          break;
        case TERRAIN_CITY:
          Ov006_SetMapCell(x, y, 450);
          break;
        case MAP_TILE(TERRAIN_CITY, 1):
          Ov006_SetMapCell(x, y, 455);
          break;
        case MAP_TILE(TERRAIN_CITY, 2):
          Ov006_SetMapCell(x, y, 460);
          break;
        case MAP_TILE(TERRAIN_HQ, 1):
          Ov006_SetMapCell(x, y, 453);
          break;
        case MAP_TILE(TERRAIN_HQ, 2):
          Ov006_SetMapCell(x, y, 458);
          break;
        case MAP_TILE(TERRAIN_BASE, 1):
          Ov006_SetMapCell(x, y, 454);
          break;
        case MAP_TILE(TERRAIN_BASE, 2):
          Ov006_SetMapCell(x, y, 459);
          break;
      }
    }
  }

  src = Ov006_gPresetMapTiles;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (MAP_TILE_TERRAIN(*src++) == TERRAIN_SHOAL) {
        CELL_AT(x, y) = 0;
        Ov006_PlaceShoal(x, y);
      }
    }
  }
}
