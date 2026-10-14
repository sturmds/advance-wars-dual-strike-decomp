#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

BOOL Player1Team_HasPlayerInGame(void);
BOOL Player_IsInGame(u8 player);
int Player_CountUnitsBuilt(int player);
int Player_CountUnitsLost(int player);
s32 _s32_div_f(s32 numerator,
               s32 denominator); /* _s32_div_f, signed division */

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Technique score (0-100) of a player's team at the end of a battle:
 * 100 plus a bonus (20 in the campaign, 10 elsewhere), minus the team's
 * units lost as a percentage of units built. 0 when player 1's team is
 * out, when the player is out, when the team built nothing or when it lost
 * more units than it built. */
int BattleScore_Technique(int player) {
  int bonus;
  int built;
  int lost;
  int i;
  int score;

  if (!Player1Team_HasPlayerInGame()) {
    return 0;
  }
  if (!Player_IsInGame(player)) {
    return 0;
  }
  /* needed to match: an if/else turns into conditional moves */
  switch (STATE->gameMode) {
    case GAME_MODE_CAMPAIGN:
      bonus = 20;
      break;
    default:
      bonus = 10;
      break;
  }
  built = 0;
  lost = 0;
  for (i = 1; i <= PLAYER_SLOT_LAST; i++) {
    if (Player_IsInGame(i) &&
        STATE->players[player].team == STATE->players[i].team) {
      built += Player_CountUnitsBuilt(i);
      lost += Player_CountUnitsLost(i);
    }
  }
  if (built == 0) {
    return 0;
  }
  if (built < lost) {
    return 0;
  }
  score = bonus + 100 - _s32_div_f(lost * 100, built);
  if (score >= 100) {
    return 100;
  }
  if (score < 0) {
    score = 0;
  }
  return score;
}
