#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/battle_save.h"
#include "game/map_data.h"
#include "game/map_header.h"
#include "game/play_records.h"

u8 Map_GetPlayerCount(u32 mapId);
const void* Map_GetName(u32 mapId);
void String_Copy(Unk_02080440_1c4* map, const void* name);
void Map_LoadData(u32 mapId);
void Map_BuildRowStarts(void);
void func_020c3e5c(void);
void TileCodeTable_Load(void);
void MapObject_Restore(
    MapObject* object,
    int index); /* copies object into MAP_STATE->objects[index] */
void TileHp_Unpack(u8* data);

/* Loads one map of a saved battle into the current game state: turn, rules
 * block, players 1-4, the first 51 units of each player and the map
 * objects. A built-in map is reloaded from its map data and then the saved
 * changed tiles are applied over it; a custom map keeps the cells it has. */
void BattleSave_LoadMap(BattleSaveMap* save) {
  int i;
  int j;
  int x;
  int y;
  BOOL customMap;

  if (data_027c027c == data_027c0288.unk_00) {
    MAP_STATE->unk_18 = 14;
  } else {
    MAP_STATE->unk_18 = 4;
  }
  *(BattleSaveTilePos*)&MAP_STATE->cursorX = save->unk_04;
  MAP_STATE->day = save->day;
  MAP_STATE->currentPlayer = save->currentPlayer;
  MAP_STATE->unk_04 = MAP_STATE->currentPlayer << 5;
  MAP_STATE->currentUnitBase =
      (MAP_STATE->currentPlayer - 1) * MAP_UNITS_PER_PLAYER;
  MAP_STATE->unk_1e = MAP_STATE->currentPlayer;
  MAP_STATE->unk_184 = save->unk_6c;
  *(BattleSaveSettings*)&MAP_STATE->gameMode = save->settings;
  *data_027c027c->unk_5c[data_027c0284] = save->unk_e34;
  gPlayRecords.visualMode = MAP_STATE->visualMode;
  gPlayRecords.unk_0e = MAP_STATE->musicReady;
  TileCodeTable_Load();
  customMap = MAP_IS_CUSTOM(MAP_STATE->unk_1c4->mapId);
  if (!customMap) {
    MAP_STATE->unk_1c4->playerCount =
        Map_GetPlayerCount(MAP_STATE->unk_1c4->mapId);
    String_Copy(MAP_STATE->unk_1c4, Map_GetName(MAP_STATE->unk_1c4->mapId));
    Map_LoadData(MAP_STATE->unk_1c4->mapId);
    Map_BuildRowStarts();
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
        MAP_STATE->mapTiles->mapCells[x + MAP_STATE->mapTiles->rowStart[y]] =
            data_0229eadc->tileCodes[x + MAP_STATE->mapTiles->rowStart[y]];
      }
    }
    func_020c3e5c();
    /* Needed to match: the original checks again. */
    customMap = MAP_IS_CUSTOM(MAP_STATE->unk_1c4->mapId);
    if (!customMap) {
      for (i = 0; save->tiles[i].cell != BATTLE_SAVE_TILES_END; i++) {
        MAP_STATE->mapTiles
            ->mapCells[save->tiles[i].x +
                       MAP_STATE->mapTiles->rowStart[save->tiles[i].y]] =
            save->tiles[i].cell;
      }
    }
  }
  for (i = 0; i < (int)ARRAY_COUNT(save->players); i++) {
    MAP_STATE->playerSlots[i + 1] = save->players[i];
  }
  MAP_STATE->playerSlots[0].armyColour = 0;
  for (i = 0; i < (int)ARRAY_COUNT(save->players); i++) {
    for (j = 0; j < BATTLE_SAVE_UNITS_PER_PLAYER; j++) {
      STATE_UNIT_SLOTS(MAP_STATE)
      [i * MAP_UNITS_PER_PLAYER + j] =
          save->units[i * BATTLE_SAVE_UNITS_PER_PLAYER + j];
    }
  }
  for (i = 0; i < BATTLE_SAVE_OBJECT_COUNT; i++) {
    MapObject_Restore(&save->objects[i], i);
  }
  TileHp_Unpack(save->unk_102e);
}
