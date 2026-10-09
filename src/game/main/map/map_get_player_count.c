#include "types.h"
#include "game/map_header.h"

u8 func_020d7ab8(u8 customIndex);

/* Returns the number of players on a map. */
u8 Map_GetPlayerCount(u32 mapId)
{
    if (MAP_IS_CUSTOM(mapId)) {
        return func_020d7ab8(mapId - MAP_CUSTOM_FIRST);
    }
    return Ov000_gMapHeaders[mapId].playerCount;
}
