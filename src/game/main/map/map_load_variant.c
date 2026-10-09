#include "types.h"
#include "game/map_header.h"

void func_020c4b48(int mapId, int variant);

/* Variant 0 loads mapId itself. Variant n > 0 loads the map id in the
 * header's variantMapIds[n - 1]. */
void Map_LoadVariant(int mapId, int variant)
{
    if (variant > 0) {
        mapId = Ov000_gMapHeaders[mapId].variantMapIds[variant - 1];
    }
    func_020c4b48(mapId, variant);
}
