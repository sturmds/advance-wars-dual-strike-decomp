#include "types.h"
#include "game/map_header.h"

extern MapHeader Ov000_gMapHeaders[];

MapHeader *MapHeader_Get(int mapId)
{
    return &Ov000_gMapHeaders[mapId];
}
