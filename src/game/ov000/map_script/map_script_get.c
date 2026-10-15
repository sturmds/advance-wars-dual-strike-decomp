/* Built without optimisation in the original (no common subexpressions, no
 * scaled-index adds); the build has one flag set for every file, so the
 * pragma stands in for it. */
#pragma optimization_level 0

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/map_script.h"

MapHeader* MapHeader_Get(int mapId);

/* The current map's script lists; NULL in game mode 4. */
MapScript* MapScript_Get(void) {
  if (MAP_STATE->gameMode == 4) {
    return NULL;
  }
  return MapHeader_Get(MAP_STATE->unk_1c4->mapId)->script;
}
