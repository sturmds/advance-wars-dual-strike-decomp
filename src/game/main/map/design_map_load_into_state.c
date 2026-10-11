#include "types.h"
#include "game/design_map.h"
#include "game/screen_other_map.h"

extern DesignMapRecord data_02291774;
extern DesignMapRecord data_02291e98;
extern u32 data_0229eaec;

void DesignMap_DecodeSlot(int slot, void *map);
void func_020d727c(DesignMapRecord *record);

/* Loads the player-made map in slot into map, then passes the record that
 * DesignMap_DecodeSlot used to func_020d727c (it reads the record from byte
 * 0x4cb on). */
void DesignMap_LoadIntoState(int slot, void *map)
{
    DesignMap_DecodeSlot(slot, map);
    if (data_0229eaec != 0 && data_027c027c == data_027c0288.unk_04) {
        func_020d727c(&data_02291e98);
    } else {
        func_020d727c(&data_02291774);
    }
}
