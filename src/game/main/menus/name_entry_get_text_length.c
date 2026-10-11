/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/Unk_022a7ea8.h"

extern u8 data_022a7ed0[];

/* Length of the text in data_022a7ed0. entry is not used. */
int NameEntry_GetTextLength(NameEntry *entry)
{
    int length = 0;

    for (;;) {
        if (data_022a7ed0[length] == 0) {
            break;
        }
        length++;
    }
    return length;
}
