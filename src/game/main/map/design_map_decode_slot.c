#include "types.h"
#include "game/design_map.h"
#include "game/screen_other_map.h"

extern DesignMapRecord data_02291774;
extern DesignMapRecord data_02291e98;
extern u32 data_0229eaec;

int Save_LoadRecord(u8 recordId, void *buffer);
void func_020d70ec(void *map, DesignMapRecord *record);

/* Reads save record 7 + slot into data_02291774 (not in link play, where the
 * buffers are already filled) and unpacks the record into map. In link play,
 * the second screen unpacks data_02291e98 instead. */
void DesignMap_DecodeSlot(int slot, void *map)
{
    if (data_0229eaec == 0) {
        Save_LoadRecord(slot + 7, &data_02291774);
    }
    if (data_0229eaec != 0 && data_027c027c == data_027c0288.unk_04) {
        func_020d70ec(map, &data_02291e98);
    } else {
        func_020d70ec(map, &data_02291774);
    }
}
