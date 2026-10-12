#include "types.h"
#include "game/design_map.h"

extern DesignMapRecord data_02291774;

int func_0201fe28(u8 recordId);
int Save_LoadRecord(u8 recordId, void* buffer);
void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);

/* Reads save record 7 + slot into dest. When func_0201fe28 reports the record
 * as unusable, only dest's player count is set to 0xff (unused). */
void DesignMap_LoadFromSlot(int slot, DesignMapRecord* dest) {
  int recordId = slot + 7;

  if (func_0201fe28(recordId)) {
    dest->playerCount = 0xff;
    return;
  }
  Save_LoadRecord(recordId, &data_02291774);
  Mem_CopyUnits(&data_02291774, dest, 0x724, 8);
}
