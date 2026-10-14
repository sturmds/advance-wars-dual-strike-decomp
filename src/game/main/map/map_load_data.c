#include "types.h"
#include "game/design_map.h"
#include "game/map_data.h"
#include "game/map_header.h"

void* Mem_Alloc(u32 size);
void Map_SetDataBuffer(void* buffer);
void Save_LoadRecord(
    int recordId,
    void*
        buffer); /* the definition takes only recordId; this call also passes r1 */
int Campaign_IsHardMode(void);
void Unpack_Lz77ByHalfwords(const void* src, void* dst); /* SDK LZ77 unpack */

/* Allocates the map data buffer and fills it for a map id. */
void Map_LoadData(u32 mapId) {
  const void* src;

  if (MAP_IS_CUSTOM(mapId)) {
    Map_SetDataBuffer(Mem_Alloc(sizeof(DesignMapRecord)));
    Save_LoadRecord(10, data_0229eadc);
    return;
  }
  Map_SetDataBuffer(Mem_Alloc(sizeof(MapDataBuffer)));
  /* Campaign_IsHardMode picks mapData (0) or hardMapData (1). */
  src = (&Ov000_gMapHeaders[mapId].mapData)[Campaign_IsHardMode()];
  if (src == NULL) {
    src = Ov000_gMapHeaders[mapId].mapData;
  }
  Unpack_Lz77ByHalfwords(src, data_0229eadc);
}
