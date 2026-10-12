/* Built without optimisation in the original (no common subexpressions, no
 * scaled-index adds); the build has one flag set for every file, so the
 * pragma stands in for it. */
#pragma optimization_level 0

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"
#include "game/map_script.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

MapHeader* MapHeader_Get(int mapId);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* The current map's script lists; NULL in game mode 4. */
MapScript* MapScript_Get(void) {
  if (GAME->gameMode == 4) {
    return NULL;
  }
  return MapHeader_Get(GAME->unk_1c4->mapId)->script;
}
