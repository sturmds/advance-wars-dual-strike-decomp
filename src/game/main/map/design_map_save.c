#include "types.h"
#include "game/design_map.h"
#include "game/save_progress.h"

extern SaveRecordBuffer data_02291774;
extern int data_0229eae4;

u8 Map_GetLoadedPlayerCount(void);
void DesignMap_BuildRecord(void* map, DesignMapRecord* record);
void Save_WriteRecord(int recordId, DesignMapRecord* record, int size);
void DesignMap_RefreshAllRows(void);

/* Converts map into the 0x724-byte save record format in data_02291774 and
 * writes it as save record 7 + slot. Outside link play, the record's player
 * count comes from the loaded map. Then refreshes the map name list. Callers
 * pass a third argument (1, or Ov006_IsMapPlayable()) that is never read. */
void DesignMap_SaveToSlot(int slot, void* map, int unused) {
  DesignMapRecord* record = &data_02291774.designMaps[0];

  DesignMap_BuildRecord(map, record);
  if (data_0229eae4 == 0) {
    record->playerCount = Map_GetLoadedPlayerCount();
  }
  Save_WriteRecord((u8)(slot + 7), &data_02291774.designMaps[0], 0x724);
  DesignMap_RefreshAllRows();
}
