/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/Unk_022a7ea8.h"

void NameEntry_GetCursorPos(NameEntry *entry, s16 *x, s16 *y)
{
    *x = entry->keys[entry->cursor].x;
    *y = entry->keys[entry->cursor].y;
}
