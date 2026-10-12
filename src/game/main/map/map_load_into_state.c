#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

void Map_LoadData(
    u16 mapId); /* needed to match: each call narrows mapId itself */
const void* Map_GetName(u16 mapId);
int Map_GetPlayerCount(u16 mapId);
void Map_LoadCustomIntoState(int mapId);
void Camera_ResetToPreset0(void* a);
void Map_BuildRowStarts(void);
void func_020c3e5c(void);
void String_Copy(Unk_02080440_1c4* map, const void* name);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8* data_0229eadc;
extern u8 data_022a7d5c[];

#define STATE (data_027c027c->unk_48[data_027c0284])

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
  Camera_ResetToPreset0(data_022a7d5c);
  STATE->unk_1c4->mapWidth = data_0229eadc[0];
  STATE->unk_1c4->mapHeight = data_0229eadc[1];
  STATE->unk_1c4->scrollX = 0;
  STATE->unk_1c4->scrollY = 0;
  STATE->unk_1c4->unk_22 = 0;
  STATE->unk_1c4->unk_24 = 0;
  STATE->unk_1c4->unk_26 = 0;
  STATE->unk_1c4->unk_28 = 0;
  STATE->unk_1c4->scrollFlags = 0;
  String_Copy(STATE->unk_1c4, Map_GetName(mapId));
  STATE->unk_1c4->playerCount = Map_GetPlayerCount(mapId);
  Map_BuildRowStarts();
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      cell = ((u16*)(data_0229eadc + 2))[x + STATE->mapTiles->rowStart[y]];
      STATE->mapTiles->mapCells[x + STATE->mapTiles->rowStart[y]] = cell;
      STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]] =
          STATE->tileCodeTerrain[cell];
    }
  }
  func_020c3e5c();
}
