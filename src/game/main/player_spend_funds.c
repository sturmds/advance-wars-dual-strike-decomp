#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"

#define FUNDS_STAT_MAX 999999999

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern PlayRecords gPlayRecords;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Takes amount from a player's funds and adds it to the player's money
 * spent. Spending by a human current player also counts in humanFundsSpent.
 * Both totals stop at 999999999. */
void Player_SpendFunds(int playerIndex, int amount) {
  if (STATE->players[STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    u32 total = gPlayRecords.humanFundsSpent + amount;

    if (total < FUNDS_STAT_MAX) {
      gPlayRecords.humanFundsSpent = total;
    } else {
      gPlayRecords.humanFundsSpent = FUNDS_STAT_MAX;
    }
  }
  STATE->players[playerIndex].funds -= amount;
  STATE->players[playerIndex].unk_04 += amount;
  if (STATE->players[playerIndex].unk_04 > FUNDS_STAT_MAX) {
    STATE->players[playerIndex].unk_04 = FUNDS_STAT_MAX;
  }
}
