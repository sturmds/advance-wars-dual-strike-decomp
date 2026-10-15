#include "types.h"
#include "game/Unk_02080440.h"
#include "game/battle_save.h"
#include "game/map_data.h"
#include "game/map_header.h"

void Map_LoadData(u32 mapId);
void func_020c3e5c(void);
void TileHp_Pack(
    u8* data); /* fills the data that BattleSave_LoadMap passes to TileHp_Unpack */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* Stores one map of the current game state into a saved battle record:
 * the reverse of BattleSave_LoadMap. For a built-in map it reloads the map
 * data and keeps only the tiles whose cell differs from it. */
void BattleSave_StoreMap(BattleSaveMap* save) {
  int i;
  int x;
  int y;
  int count;
  u16 cell;
  BOOL customMap;

  save->unk_04 = *(BattleSaveTilePos*)&MAP_STATE->cursorX;
  save->day = MAP_STATE->day;
  save->currentPlayer = MAP_STATE->currentPlayer;
  save->unk_6c = MAP_STATE->unk_184;
  save->settings = *(BattleSaveSettings*)&MAP_STATE->gameMode;
  save->unk_e34 = *data_027c027c->unk_5c[data_027c0284];
  customMap = MAP_IS_CUSTOM(MAP_STATE->unk_1c4->mapId);
  if (!customMap) {
    Map_LoadData(MAP_STATE->unk_1c4->mapId);
    count = 0;
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
        cell =
            MAP_STATE->mapTiles->mapCells[x + MAP_STATE->mapTiles->rowStart[y]];
        if (cell !=
            data_0229eadc->tileCodes[x + MAP_STATE->mapTiles->rowStart[y]]) {
          save->tiles[count].cell = cell;
          save->tiles[count].x = x;
          save->tiles[count].y = y;
          count++;
        }
      }
    }
    save->tiles[count].cell = BATTLE_SAVE_TILES_END;
    func_020c3e5c();
  }
  /* Needed to match: x doubles as the player and unit loop counter. */
  for (x = 0; x < (int)ARRAY_COUNT(save->players); x++) {
    save->players[x] = MAP_STATE->playerSlots[x + 1];
  }
  for (x = 0; x < (int)ARRAY_COUNT(save->players); x++) {
    for (i = 0; i < BATTLE_SAVE_UNITS_PER_PLAYER; i++) {
      save->units[x * BATTLE_SAVE_UNITS_PER_PLAYER + i] =
          STATE_UNIT_SLOTS(MAP_STATE)[x * MAP_UNITS_PER_PLAYER + i];
    }
  }
  for (i = 0; i < BATTLE_SAVE_OBJECT_COUNT; i++) {
    save->objects[i] = MAP_STATE->objects[i];
  }
  TileHp_Pack(save->unk_102e);
}
