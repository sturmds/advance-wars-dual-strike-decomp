/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"

u8* func_021016e4(void);

void NameEntry_DeleteChar(NameEntry* entry) {
  int pos = entry->textLength;
  u8* text = func_021016e4();

  if (entry->textLength == entry->maxLength * 2 - 1) {
    if (text[pos] == 0) {
      pos--;
      entry->textLength--;
    }
  } else if (entry->textLength != 0) {
    pos--;
    entry->textLength--;
  }
  text[pos] = 0;
}
