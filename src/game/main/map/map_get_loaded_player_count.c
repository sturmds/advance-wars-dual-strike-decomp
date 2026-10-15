#include "types.h"
#include "game/Unk_02080440.h"

/* Number of players on the map that is loaded. */
u8 Map_GetLoadedPlayerCount(void) {
  return data_027c027c->unk_48[data_027c0284]->unk_1c4->playerCount;
}
