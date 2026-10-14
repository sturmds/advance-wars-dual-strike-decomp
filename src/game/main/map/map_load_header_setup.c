#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define CURRENT_MAP_HEADER (Ov000_gMapHeaders[GAME->unk_1c4->mapId])

/* Copies each player's army and team, and the Fog of War flag, from the
 * current map's header into the game state. Only when gameMode is 0, 1 or 3.
 * Callers pass the map id, but the function reads it from the state. */
void Map_LoadHeaderSetup(int mapId) {
  int player;
  u8 mode;

  GAME->unk_1c4->unk_30 = 0;

  mode = GAME->gameMode;
  if (mode != 0 && mode != 1 && mode != 3) {
    return;
  }

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    GAME->team[player] = CURRENT_MAP_HEADER.team[player];
    GAME->army[player] = CURRENT_MAP_HEADER.army[player];
  }

  if (CURRENT_MAP_HEADER.fog) {
    GAME->unk_1c4->fog = 1;
  }
}
