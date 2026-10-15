#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map.h"

extern Unk_02080440_48 data_02183b08;

void String_Copy(char* dest, const char* src);
void Map_BuildRowStarts(void);

/* Copies a player-made map's record into the current map state: its name
 * into name, the player count, the map size and every tile's cell code.
 * The army colours are copied only for the first screen's state. */
void DesignMap_UnpackRecord(char* name, DesignMapRecord* record) {
  int i;
  int cell;
  int x;
  int y;

  String_Copy(name, record->name);
  MAP_STATE->unk_1c4->playerCount = record->playerCount;
  if (MAP_STATE == &data_02183b08) {
    for (i = 0; i < ARMY_SLOT_COUNT; i++) {
      MAP_STATE->army[i] = record->armyColours[i];
    }
  }
  MAP_STATE->unk_1c4->mapWidth = record->width;
  MAP_STATE->unk_1c4->mapHeight = record->height;
  Map_BuildRowStarts();
  cell = 0;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      MAP_STATE->mapTiles->mapCells[MAP_STATE->mapTiles->rowStart[y] + x] =
          record->tileCodes[cell++];
    }
  }
}
