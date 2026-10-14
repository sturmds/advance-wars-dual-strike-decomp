/* The original was built without optimization: no value is reused from a
 * register, every global is read again. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"

void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);

NameEntry* gNameEntry;
u8 gNameEntryText[NAME_ENTRY_TEXT_SIZE];
u8 gNameEntryStartText[NAME_ENTRY_TEXT_SIZE];

/* First step of the name entry screen: allocates and clears the work block. */
void NameEntry_AllocWork(NameEntryTask* task) {
  task->work = Mem_Alloc(sizeof(NameEntry));
  gNameEntry = task->work;
  Mem_Zero32(0, gNameEntry, sizeof(NameEntry));
}
