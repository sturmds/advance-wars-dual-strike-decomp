/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry.h"
#include "game/text_print.h"

void Text_AddSpacerCodes(const void* src, void* dst);

extern u8 gNameEntryText[];

void NameEntry_DrawText(NameEntry* entry) {
  int x = 12;
  u8 converted[0x6c];

  if (entry->maxLength == 16) {
    x -= 4;
  }
  if (entry->maxLength == 8) {
    Text_Print(x, 1, NameEntry_GetTextMap(), gNameEntryText, 0, 0x200);
  } else {
    Text_AddSpacerCodes(gNameEntryText, converted);
    Text_Print(x, 1, NameEntry_GetTextMap(), converted, 0, 0x200);
  }
}
