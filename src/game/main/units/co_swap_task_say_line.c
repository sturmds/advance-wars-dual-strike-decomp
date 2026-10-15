#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern u32 data_02153640[][136];

int Rng_Next(void);
void CoQuote_StartText(u32 textId);

#define FRONT_CO(state) ((state)->players[(state)->currentPlayer].front)

/* Step of the CO swap script (data_021684f8, data_02168578): shows a line of
 * text for the player's new front CO. A CO byte with bit 7 set gets the fixed
 * text 0xf000028a; the others pick one of two texts at random from
 * data_02153640 (name guessed: a text id table, 136 entries per CO). */
void CoSwapTask_ShowLine(void) {
  if (FRONT_CO(MAP_STATE).id & 0x80) {
    CoQuote_StartText(0xf000028a);
    return;
  }
  CoQuote_StartText(data_02153640[FRONT_CO(MAP_STATE).id & PLAYER_CO_ID_MASK]
                                 [Rng_Next() & 1]);
}
