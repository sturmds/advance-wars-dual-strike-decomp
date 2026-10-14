#include "types.h"
#include "game/map_header.h"
#include "game/screen_other_map.h"
#include "game/save_progress.h"

extern SaveRecordBuffer data_02291774;

int Save_LoadRecord(u8 recordId, void* buffer);
void BattleSave_Unpack(void);
void Map_RefreshAfterRestore(void);
void DesignMap_DecodeSlot(int slot, void* map);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Loads battle save record recordId (0: none) into data_02291774, applies it
 * with BattleSave_Unpack, then on the current screen and, in a two-map battle, on
 * the other screen: unpacks a player-made map again from design map slot 3
 * or 4 and runs Map_RefreshAfterRestore. */
void BattleSave_Restore(int recordId) {
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;
  Unk_02080440_1c4* map;
  BOOL customMap;

  if (recordId == 0) {
    return;
  }
  Save_LoadRecord(recordId, &data_02291774);
  BattleSave_Unpack();

  /* the current screen */
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
  saved = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
  map = STATE->unk_1c4;
  customMap = MAP_IS_CUSTOM(map->mapId);
  if (customMap) {
    DesignMap_DecodeSlot(3, map);
  }
  Map_RefreshAfterRestore();
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);

  if (STATE->secondMapId == 0) {
    return;
  }

  /* the other screen */
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
  saved = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
  map = STATE->unk_1c4;
  customMap = MAP_IS_CUSTOM(map->mapId);
  if (customMap) {
    DesignMap_DecodeSlot(4, map);
  }
  Map_RefreshAfterRestore();
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
}
