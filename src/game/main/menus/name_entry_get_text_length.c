/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"

extern u8 gNameEntryText[];

/* Length of the text in gNameEntryText. entry is not used. */
int NameEntry_GetTextLength(NameEntry* entry) {
  int length = 0;

  for (;;) {
    if (gNameEntryText[length] == 0) {
      break;
    }
    length++;
  }
  return length;
}
