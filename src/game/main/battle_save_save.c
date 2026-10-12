#include "types.h"
#include "game/battle_save.h"
#include "game/map_header.h"
#include "game/screen_other_map.h"

extern u8 data_02291774[];
extern u32 gLinkPlay;

void func_02019920(void);
u8 func_020193bc(int recordId);
void BattleSave_SetRecordFlag(int recordId, int value);
void DesignMap_SaveToSlot(int slot, void* map, int unused);
void BattleSave_Pack(u8 flag);
void Save_WriteRecord(u8 recordId, void* buffer, int size);
void* PlayRecords_TakeSnapshot(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Saves the battle as save record recordId (1 to 6; 0 does nothing; never in
 * link play). Sets the record's byte (func_020193e0) to 1 if it is 0, writes a
 * player-made map on either screen to design map slot 3 (current screen) or
 * 4 (the second map of a two-map battle), packs the battle into
 * data_02291774 with BattleSave_Pack and writes it. In game mode 0 it also
 * calls PlayRecords_TakeSnapshot, which copies two words of gPlayRecords. */
void BattleSave_Save(int recordId, int flag) {
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;
  Unk_02080440_1c4* map;
  BOOL customMap;
  int secondMapId;

  if (recordId == 0) {
    return;
  }
  if (gLinkPlay != 0) {
    return;
  }
  func_02019920();
  if (func_020193bc(recordId) == 0) {
    BattleSave_SetRecordFlag(recordId, 1);
  }

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
  saved = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
  map = STATE->unk_1c4;
  customMap = MAP_IS_CUSTOM(map->mapId);
  if (customMap) {
    DesignMap_SaveToSlot(3, map, 1);
  }
  secondMapId = STATE->secondMapId;
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
  saved = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
  if (secondMapId != 0 && MAP_IS_CUSTOM(secondMapId)) {
    DesignMap_SaveToSlot(4, STATE->unk_1c4, 1);
  }
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);

  BattleSave_Pack(flag);
  Save_WriteRecord(recordId, data_02291774, BATTLE_SAVE_RECORD_SIZE);
  if (STATE->gameMode == 0) {
    PlayRecords_TakeSnapshot();
  }
}
