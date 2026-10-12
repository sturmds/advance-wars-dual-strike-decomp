#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

BOOL Player1Team_HasPlayerInGame(void);
BOOL Player_IsInGame(u8 player);
int Player_CountUnitsBuilt(int player);
s32 HwDiv32(s32 numerator, s32 denominator);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Power score (0-100) of a player at the end of a battle: Player.mostTurnKills
 * (most kills in one turn, per doc 890) times 1000, divided by the units
 * the other teams built, capped at 100. 0 when player 1's team is out,
 * when the player is out or when no enemy built a unit. */
int BattleScore_Power(int player) {
  int i;
  int enemyBuilt;
  int score;

  if (!Player1Team_HasPlayerInGame()) {
    return 0;
  }
  if (!Player_IsInGame(player)) {
    return 0;
  }
  enemyBuilt = 0;
  for (i = 1; i < 5; i++) {
    if (STATE->players[i].controller != PLAYER_CONTROLLER_NONE &&
        STATE->players[player].team != STATE->players[i].team) {
      enemyBuilt += Player_CountUnitsBuilt(i);
    }
  }
  if (enemyBuilt == 0) {
    return 0;
  }
  score = HwDiv32(STATE->players[player].mostTurnKills * 1000, enemyBuilt);
  if (score > 100) {
    score = 100;
  }
  return score;
}
