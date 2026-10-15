#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7a9c.h"
#include "game/win_lose.h"

BOOL Player_IsInGame(u8 player);

/* Sorts the players with a controller into the win/lose screen's sides:
 * side A for players still in the game, side B for the others, at most 4
 * each. Then copies the points total and the points shown from
 * data_022a7a9c. */
void WinLoseSides_Fill(void) {
  int i;
  int player;

  for (i = 0; i < (int)ARRAY_COUNT(data_022a7a88.sideA); i++) {
    data_022a7a88.sideA[i] = 0;
    data_022a7a88.sideB[i] = 0;
  }
  data_022a7a88.countA = 0;
  data_022a7a88.countB = 0;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (data_027c027c->unk_48[data_027c0284]->players[player].controller !=
        PLAYER_CONTROLLER_NONE) {
      if (Player_IsInGame(player)) {
        if (data_022a7a88.countA >= 4) {
          break;
        }
        data_022a7a88.sideA[data_022a7a88.countA++] = player;
      } else {
        if (data_022a7a88.countB >= 4) {
          break;
        }
        data_022a7a88.sideB[data_022a7a88.countB++] = player;
      }
    }
  }
  data_022a7a88.unk_0c = data_022a7a9c.totalPoints;
  data_022a7a88.unk_10 = data_022a7a9c.unk_10;
}
