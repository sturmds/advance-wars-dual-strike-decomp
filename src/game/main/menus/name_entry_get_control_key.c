/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"

/* Index of the control key under the cursor, or -1 when it is on a
 * character key. */
int NameEntry_GetControlKey(NameEntry* entry) {
  if (entry->cursor < NAME_ENTRY_KEY_COUNT) {
    return -1;
  }
  return entry->cursor - NAME_ENTRY_KEY_COUNT;
}
