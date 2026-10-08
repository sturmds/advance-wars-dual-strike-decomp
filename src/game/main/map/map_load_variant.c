#include "types.h"
#include "game/map_header.h"

extern MapHeader Ov000_gMapHeaders[];

void func_020c4b48(int mapId, int variant);

/* Variant 0 loads mapId itself. Variant n > 0 loads another map id from
 * the header: a u16 list that starts at unk_10 (+0x10), entry n - 1.
 * The header names only its first entry so far, hence the pointer. */
void Map_LoadVariant(int mapId, int variant)
{
    if (variant > 0) {
        mapId = (&Ov000_gMapHeaders[mapId].unk_10)[variant - 1];
    }
    func_020c4b48(mapId, variant);
}
