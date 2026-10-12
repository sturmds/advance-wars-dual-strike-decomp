/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"

/* Index of the key at position (x, y), or -1 when no used key is there. */
int NameEntry_FindKey(NameEntry* entry, s16 x, s16 y) {
  int i;

  for (i = 0; i < NAME_ENTRY_KEY_COUNT; i++) {
    NameEntryKey* key = &entry->keys[i];

    if (key->unk_00 != 0 && key->x == x && key->y == y) {
      return i;
    }
  }
  return -1;
}
