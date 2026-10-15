#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

BOOL Player1Team_HasPlayerInGame(void);
BOOL Player_IsInGame(u8 player);
int Player_CountUnitsBuilt(int player);

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
  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (MAP_STATE->players[i].controller != PLAYER_CONTROLLER_NONE &&
        MAP_STATE->players[player].team != MAP_STATE->players[i].team) {
      enemyBuilt += Player_CountUnitsBuilt(i);
    }
  }
  if (enemyBuilt == 0) {
    return 0;
  }
  score = HwDiv32(MAP_STATE->players[player].mostTurnKills * 1000, enemyBuilt);
  if (score > 100) {
    score = 100;
  }
  return score;
}
