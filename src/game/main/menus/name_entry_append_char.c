/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/text_print.h"

u8* func_021016e4(void);

extern u8* data_0216ad5c[];

/* Types the character under the cursor. Returns 0 when the text is full
 * (16 characters, or wider than maxLength * 8 pixels) and nothing is added. */
BOOL NameEntry_AppendChar(NameEntry* entry) {
  u8* dest = func_021016e4() + entry->textLength;
  u8 ch = *data_0216ad5c[entry->cursor];

  if (entry->textLength >= 16) {
    return FALSE;
  }
  *dest = ch;
  if (entry->maxLength * 8 <= Text_GetPrintWidth(func_021016e4())) {
    *dest = 0;
    return FALSE;
  }
  entry->textLength++;
  return TRUE;
}
