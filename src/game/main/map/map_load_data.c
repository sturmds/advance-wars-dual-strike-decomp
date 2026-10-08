#include "types.h"
#include "game/map_header.h"

extern MapHeader Ov000_gMapHeaders[];
extern void *data_0229eadc;

void *func_0200bf40(u32 size);
void func_020d3c34(void *buffer);
void func_0201febc(int a, void *buffer);
int func_021039e4(void);
void func_0210f78c(const void *src, void *dst); /* SDK LZ77 unpack */

/* Allocates the map data buffer and fills it for a map id. */
void Map_LoadData(u32 mapId)
{
    const void *src;

    if (MAP_IS_CUSTOM(mapId)) {
        func_020d3c34(func_0200bf40(0x724));
        func_0201febc(10, data_0229eadc);
        return;
    }
    func_020d3c34(func_0200bf40(0xa14));
    /* func_021039e4 picks mapData (0) or hardMapData (1). */
    src = (&Ov000_gMapHeaders[mapId].mapData)[func_021039e4()];
    if (src == NULL) {
        src = Ov000_gMapHeaders[mapId].mapData;
    }
    func_0210f78c(src, data_0229eadc);
}
