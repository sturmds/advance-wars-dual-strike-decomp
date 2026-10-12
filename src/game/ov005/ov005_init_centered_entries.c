#include "types.h"
#include "game/Unk_ov005_0235b6ec.h"

void Mem_Copy32(const void* src, void* dst, u32 size);

/* Copies the entry templates and centers each entry horizontally; y is moved
 * down by 256 (purpose not known). */
void Ov005_InitCenteredEntries(Unk_ov005_0235b6ec* work) {
  Unk_ov005_0235b6ec_Entry* entry = work->entries;

  Mem_Copy32(work->templates, entry, sizeof(work->entries));
  for (;;) {
    if (entry->unk_00 == 0) {
      return;
    }
    entry->x = (256 - entry->charCount * 8) >> 1;
    entry->y += 256;
    entry++;
  }
}
