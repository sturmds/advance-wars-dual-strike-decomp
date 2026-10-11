#include "types.h"
#include "game/design_map.h"

extern DesignMapRecord data_02291774;
extern u32 data_0229eae4;

u8 Map_GetLoadedPlayerCount(void);
void func_020d73d4(void *map, DesignMapRecord *record);
void Save_WriteRecord(int recordId, DesignMapRecord *record, int size);
void func_020d6fcc(void);

/* Converts map into the 0x724-byte save record format in data_02291774 and
 * writes it as save record 7 + slot. Outside link play, the record's player
 * count comes from the loaded map. Then refreshes the map name list. */
void DesignMap_SaveToSlot(int slot, void *map)
{
    DesignMapRecord *record = &data_02291774;

    func_020d73d4(map, record);
    if (data_0229eae4 == 0) {
        record->playerCount = Map_GetLoadedPlayerCount();
    }
    Save_WriteRecord((u8)(slot + 7), &data_02291774, 0x724);
    func_020d6fcc();
}
