#include "types.h"
#include "game/design_map.h"
#include "game/save_progress.h"

void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);
void Save_WriteRecord(int recordId, DesignMapRecord* record, int size);
void DesignMap_RefreshAllRows(void);

/* Writes record as save record 7 + slot (through the data_02291774 buffer)
 * and refreshes the map name list. */
void DesignMap_SaveRecordToSlot(int slot, DesignMapRecord* record) {
  if (record != &data_02291774.designMaps[0]) {
    Mem_CopyUnits(record, &data_02291774, sizeof(DesignMapRecord), 8);
  }
  Save_WriteRecord((u8)(slot + 7), &data_02291774.designMaps[0], 0x724);
  DesignMap_RefreshAllRows();
}
