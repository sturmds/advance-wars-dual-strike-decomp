#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

u32 Rng_Next(void);
void CoQuote_StartText(u32 script);

/* Starts one of the six power quotes of the player's front CO, at random. CO ids with bit 7 set get a fixed line. Returns 0. */
int CoQuote_StartRandom(int player) {
  if (MAP_STATE->players[player].front.id & 0x80) {
    CoQuote_StartText(0xf000028a);
  } else {
    CoQuote_StartText(((TextRef*)CO_TABLE_FIELD(
        MAP_STATE->players[player].front.id & PLAYER_CO_ID_MASK,
        powerQuotes))[Rng_Next() % 6]
                          .id);
  }
  return 0;
}
