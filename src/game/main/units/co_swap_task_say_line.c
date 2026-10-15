#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

int Rng_Next(void);
void CoQuote_StartText(u32 textId);

#define FRONT_CO(state) ((state)->players[(state)->currentPlayer].front)

/* Step of the CO swap script (data_021684f8, data_02168578): shows a line of
 * text for the player's new front CO. A CO byte with bit 7 set gets the fixed
 * text 0xf000028a; the others pick one of the CO's two switch quotes at
 * random. */
void CoSwapTask_ShowLine(void) {
  if (FRONT_CO(MAP_STATE).id & 0x80) {
    CoQuote_StartText(0xf000028a);
    return;
  }
  CoQuote_StartText(((TextRef*)CO_TABLE_FIELD(
      FRONT_CO(MAP_STATE).id & PLAYER_CO_ID_MASK, switchQuotes))[Rng_Next() & 1]
                        .id);
}
