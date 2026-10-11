/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"

#define NAME_ENTRY_TEXT_SIZE 34

void Mem_Zero32(u32 value, void *dst, u32 size);

extern u8 data_022a7ed0[];

void NameEntry_ClearBuffer(void)
{
    Mem_Zero32(0, data_022a7ed0, NAME_ENTRY_TEXT_SIZE);
}
