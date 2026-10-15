#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/camera_pan.h"
#include "game/map_data.h"

void Map_LoadData(
    u16 mapId); /* needed to match: each call narrows mapId itself */
const void* Map_GetName(u16 mapId);
int Map_GetPlayerCount(u16 mapId);
void Map_LoadCustomIntoState(int mapId);
void Camera_ResetToPreset0(void* a);
void Map_BuildRowStarts(void);
void func_020c3e5c(void);
void String_Copy(Unk_02080440_1c4* map, const void* name);

extern Camera data_022a7d5c;

/* Loads a map's data into the game state: size, name, player count, and
 * every tile (the cell code is kept in mapCells, converted through
 * tileCodeTerrain into tiles). Frees the map data buffer at the end. Custom
 * maps go through Map_LoadCustomIntoState. */
void Map_LoadIntoState(int mapId) {
  int x;
  int y;
  u16 cell;

  if (MAP_IS_CUSTOM(mapId)) {
    Map_LoadCustomIntoState(mapId);
    return;
  }
  Map_LoadData(mapId);
  Camera_ResetToPreset0(&data_022a7d5c);
  MAP_STATE->unk_1c4->mapWidth = data_0229eadc->width;
  MAP_STATE->unk_1c4->mapHeight = data_0229eadc->height;
  MAP_STATE->unk_1c4->scrollX = 0;
  MAP_STATE->unk_1c4->scrollY = 0;
  MAP_STATE->unk_1c4->unk_22 = 0;
  MAP_STATE->unk_1c4->unk_24 = 0;
  MAP_STATE->unk_1c4->unk_26 = 0;
  MAP_STATE->unk_1c4->unk_28 = 0;
  MAP_STATE->unk_1c4->scrollFlags = 0;
  String_Copy(MAP_STATE->unk_1c4, Map_GetName(mapId));
  MAP_STATE->unk_1c4->playerCount = Map_GetPlayerCount(mapId);
  Map_BuildRowStarts();
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      cell = data_0229eadc->tileCodes[x + MAP_STATE->mapTiles->rowStart[y]];
      MAP_STATE->mapTiles->mapCells[x + MAP_STATE->mapTiles->rowStart[y]] =
          cell;
      MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] =
          MAP_STATE->tileCodeTerrain[cell];
    }
  }
  func_020c3e5c();
}
