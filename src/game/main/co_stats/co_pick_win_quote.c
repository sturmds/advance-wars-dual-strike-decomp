#include "types.h"
#include "game/co_data.h"
#include "game/player.h"

u32 Rng_Next(void);

/* Text id of one of the CO's three win quotes, picked at random. */
u32 Co_PickWinQuote(int co) {
  return gCoTable[co & PLAYER_CO_ID_MASK].winQuotes[Rng_Next() % 3].id;
}
