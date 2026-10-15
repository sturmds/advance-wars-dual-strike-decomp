#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"

#define FUNDS_STAT_MAX 999999999

/* Takes amount from a player's funds and adds it to the player's money
 * spent. Spending by a human current player also counts in humanFundsSpent.
 * Both totals stop at 999999999. */
void Player_SpendFunds(int playerIndex, int amount) {
  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    u32 total = gPlayRecords.humanFundsSpent + amount;

    if (total < FUNDS_STAT_MAX) {
      gPlayRecords.humanFundsSpent = total;
    } else {
      gPlayRecords.humanFundsSpent = FUNDS_STAT_MAX;
    }
  }
  MAP_STATE->players[playerIndex].funds -= amount;
  MAP_STATE->players[playerIndex].unk_04 += amount;
  if (MAP_STATE->players[playerIndex].unk_04 > FUNDS_STAT_MAX) {
    MAP_STATE->players[playerIndex].unk_04 = FUNDS_STAT_MAX;
  }
}
