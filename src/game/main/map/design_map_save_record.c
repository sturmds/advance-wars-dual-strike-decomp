#include "types.h"
#include "game/design_map.h"

extern DesignMapRecord data_02291774;

void Mem_CopyUnits(const void *src, void *dst, u32 size, u32 unitBits);
void func_020200fc(int recordId, DesignMapRecord *record, int size);
void func_020d6fcc(void);

/* Writes record as save record 7 + slot (through the data_02291774 buffer)
 * and refreshes the map name list. */
void DesignMap_SaveRecordToSlot(int slot, DesignMapRecord *record)
{
    if (record != &data_02291774) {
        Mem_CopyUnits(record, &data_02291774, 0x724, 8);
    }
    func_020200fc((u8)(slot + 7), &data_02291774, 0x724);
    func_020d6fcc();
}
