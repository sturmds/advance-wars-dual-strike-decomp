#include "types.h"
#include "game/Unk_ov003_02369bb8.h"

extern Unk_ov003_02369bb8 data_ov003_02369bb8[4];

/* Clears unk_09_0 of every busy entry. */
void func_ov003_0235e9b8(void)
{
    Unk_ov003_02369bb8 *entry = data_ov003_02369bb8;
    int i;

    for (i = 0; i < 4; i++, entry++) {
        if (entry->busy) {
            entry->unk_09_0 = 0;
        }
    }
}
