/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry.h"

void func_01ff8a2c(u16 *map, int x, int y, int width, int height, int tile);

/* Blanks the two map rows that the text is printed into. */
void NameEntry_ClearText(NameEntry *entry)
{
    int x = 12;

    if (entry->maxLength == 16) {
        x -= 4;
    }
    func_01ff8a2c(NameEntry_GetTextMap(), x, 1, entry->maxLength, 2, 0);
}
