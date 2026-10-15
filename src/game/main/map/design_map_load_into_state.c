#include "types.h"
#include "game/design_map.h"
#include "game/screen_other_map.h"
#include "game/save_progress.h"

extern DesignMapRecord data_02291e98;
extern u32 gLinkPlay;

void DesignMap_DecodeSlot(int slot, void* map);
void DesignMap_SpawnUnits(DesignMapRecord* record);

/* Loads the player-made map in slot into map, then passes the record that
 * DesignMap_DecodeSlot used to DesignMap_SpawnUnits (it reads the record from byte
 * 0x4cb on). */
void DesignMap_LoadIntoState(int slot, void* map) {
  DesignMap_DecodeSlot(slot, map);
  if (gLinkPlay != 0 && data_027c027c == data_027c0288.unk_04) {
    DesignMap_SpawnUnits(&data_02291e98);
  } else {
    DesignMap_SpawnUnits(&data_02291774.designMaps[0]);
  }
}
