#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

#define CURRENT_MAP_HEADER (Ov000_gMapHeaders[MAP_STATE->unk_1c4->mapId])

/* Copies each player's army and team, and the Fog of War flag, from the
 * current map's header into the game state. Only when gameMode is 0, 1 or 3.
 * Callers pass the map id, but the function reads it from the state. */
void Map_LoadHeaderSetup(int mapId) {
  int player;
  u8 mode;

  MAP_STATE->unk_1c4->unk_30 = 0;

  mode = MAP_STATE->gameMode;
  if (mode != 0 && mode != 1 && mode != 3) {
    return;
  }

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    MAP_STATE->team[player] = CURRENT_MAP_HEADER.team[player];
    MAP_STATE->army[player] = CURRENT_MAP_HEADER.army[player];
  }

  if (CURRENT_MAP_HEADER.fog) {
    MAP_STATE->unk_1c4->fog = 1;
  }
}
