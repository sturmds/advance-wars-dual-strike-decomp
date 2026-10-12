#include "types.h"
#include "game/map_header.h"
#include "game/text_ref.h"

const void* DesignMap_GetName(u8 customIndex);
const void* Text_Get(u32 textId);

/* Returns the name string of a map. */
const void* Map_GetName(u32 mapId) {
  if (MAP_IS_CUSTOM(mapId)) {
    return DesignMap_GetName(mapId - MAP_CUSTOM_FIRST);
  }
  return Text_Get(Ov000_gMapHeaders[mapId].name.id);
}
