/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
void Mem_Copy32(const void* src, void* dst, u32 size);

extern NameEntryKey data_0216afb4[NAME_ENTRY_KEY_COUNT];
extern u8 data_0216aae0[0x24];

void NameEntry_LoadKeyTable(NameEntry* entry) {
  Mem_Copy32(data_0216afb4, entry->keys, sizeof(entry->keys));
  Mem_Copy32(data_0216aae0, entry->unk_4b8, sizeof(entry->unk_4b8));
}
