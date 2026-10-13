/* The original was built without optimization: no value is reused from a
 * register, every global is read again. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"

#define NAME_ENTRY_WORK_SIZE 0x4f4

void* Mem_Alloc(u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);

NameEntry* gNameEntry;
u8 gNameEntryText[0x24];      /* size: up to the next symbol */
u8 gNameEntryStartText[0x24]; /* size: up to the next symbol */

/* First step of the name entry screen: allocates and clears the work block. */
void NameEntry_AllocWork(NameEntryTask* task) {
  task->work = Mem_Alloc(NAME_ENTRY_WORK_SIZE);
  gNameEntry = task->work;
  Mem_Zero32(0, gNameEntry, NAME_ENTRY_WORK_SIZE);
}
