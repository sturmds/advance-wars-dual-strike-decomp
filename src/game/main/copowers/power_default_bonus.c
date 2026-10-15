#include "types.h"
#include "game/Unk_02080440.h"

/* Every CO power gives +10 firepower and +10 defense to the current player. */
void PowerDefaultBonus(void) {
  data_027c027c->unk_48[data_027c0284]
      ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
      .firepowerBonus = 10;
  data_027c027c->unk_48[data_027c0284]
      ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
      .defenseBonus = 10;
}
