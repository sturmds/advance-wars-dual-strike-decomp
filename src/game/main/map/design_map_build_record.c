#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map.h"
#include "game/terrain.h"

void String_Copy(char* dest, const char* src);

extern s8 Ov000_gUrbanTerrain
    []; /* nonzero for urban terrain, indexed by terrain id */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* Writes the loaded map into a design map record: size, name, the cell code
 * and unit of each tile, urban tile counts and the army colours. With
 * countHqs set to 1 the player count is the number of HQ tiles, else 0. */
void DesignMap_BuildRecord(const char* name, DesignMapRecord* record,
                           int countHqs) {
  u8 urbanCounts[ARMY_SLOT_COUNT + 1]; /* per owner slot, then the total */
  u16 hqs;
  int x;
  u8 terrainTile;
  int terrain;
  u8 ownerCodes[ARMY_COLOUR_COUNT];
  int y;
  u16 tile;
  u16 i;
  u8 unitAt;
  Unk_02080440_48* state;

  String_Copy(record->name, name);
  record->unk_4c2 = 0;
  record->width = MAP_STATE->unk_1c4->mapWidth;
  record->height = MAP_STATE->unk_1c4->mapHeight;
  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    record->armyColours[i] = 0;
  }
  for (i = 0; i < ARRAY_COUNT(urbanCounts); i++) {
    urbanCounts[i] = 0;
  }
  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    ownerCodes[MAP_STATE->army[i]] = (i - 1) << 6;
  }

  tile = 0;
  hqs = 0;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    state = MAP_STATE;
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      unitAt = state->mapTiles->unitAt[x + state->mapTiles->rowStart[y]];
      if (unitAt != 0) {
        record->units[tile] =
            state->units[unitAt].type | ownerCodes[(unitAt >> 6) + 1];
      } else {
        record->units[tile] = 0;
      }
      record->tileCodes[tile++] =
          MAP_STATE->mapTiles->mapCells[MAP_TILE_INDEX(x, y)];
      state = MAP_STATE;
      terrainTile = state->mapTiles->tiles[x + state->mapTiles->rowStart[y]];
      terrain = MAP_TILE_TERRAIN(terrainTile);
      if (Ov000_gUrbanTerrain[terrain] != 0) {
        urbanCounts[ARMY_SLOT_COUNT]++;
        urbanCounts[terrainTile >> 5]++;
      }
      if (terrain == TERRAIN_HQ) {
        hqs++;
      }
    }
  }

  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    if (urbanCounts[0] < urbanCounts[i]) {
      urbanCounts[0] = urbanCounts[i];
    }
  }
  record->propertyCount = urbanCounts[ARMY_SLOT_COUNT];
  record->unk_4ca = urbanCounts[0] + 1;
  if (countHqs == 1) {
    record->playerCount = hqs;
  } else {
    record->playerCount = 0;
  }
  for (i = 0; i < ARMY_SLOT_COUNT; i++) {
    record->armyColours[i] = MAP_STATE->army[i];
  }
}
