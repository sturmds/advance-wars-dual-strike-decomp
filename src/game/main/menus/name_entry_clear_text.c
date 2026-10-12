/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry.h"

void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);

/* Blanks the two map rows that the text is printed into. */
void NameEntry_ClearText(NameEntry* entry) {
  int x = 12;

  if (entry->maxLength == 16) {
    x -= 4;
  }
  Bg_FillRect(NameEntry_GetTextMap(), x, 1, entry->maxLength, 2, 0);
}
