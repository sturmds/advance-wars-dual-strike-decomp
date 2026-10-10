#include "types.h"
#include "game/Unk_ov003_02369bb8.h"

extern Unk_ov003_02369bb8 data_ov003_02369bb8[4];

/* Finds the busy entry covering a tile: the tile itself for kind 6, the 3 by 3
 * tiles around it for kind 7. */
Unk_ov003_02369bb8 *Ov003_FindEntryAtTile(int x, int y)
{
    Unk_ov003_02369bb8 *entry = data_ov003_02369bb8;
    int i;

    for (i = 0; i < 4; i++, entry++) {
        if (!entry->busy) {
            continue;
        }
        switch (entry->unk_08_0) {
        case 6:
            if (x == entry->x && y == entry->y) {
                return entry;
            }
            break;
        case 7:
            if (x >= entry->x - 1 && y >= entry->y - 1 && x <= entry->x + 1 && y <= entry->y + 1) {
                return entry;
            }
            break;
        }
    }
    return NULL;
}
