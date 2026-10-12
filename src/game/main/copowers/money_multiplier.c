#include "types.h"
#include "game/Unk_02080440.h"

void PowerDefaultBonus(void);
s32 HwDiv32(s32 numerator, s32 denominator);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Colin's Power of Money: +1 firepower for every 300 funds, on top of the
 * default power bonus. */
void MoneyMultiplier(void) {
  PowerDefaultBonus();
  /* needed to match: one expression, no local for the battle state */
  data_027c027c->unk_48[data_027c0284]
      ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
      .firepowerBonus +=
      HwDiv32(data_027c027c->unk_48[data_027c0284]
                  ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
                  .funds,
              300);
}
