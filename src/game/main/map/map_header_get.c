#include "types.h"
#include "game/map_header.h"

MapHeader* MapHeader_Get(int mapId) { return &Ov000_gMapHeaders[mapId]; }
