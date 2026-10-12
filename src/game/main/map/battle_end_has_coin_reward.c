#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

BOOL Player1Team_HasPlayerInGame(void);

/* TRUE when gLinkPlay is set, in game mode 2, or when player 1's team is
 * still in the game. BattleEnd_AwardCoins adds coins only then. The flag's
 * meaning is unknown. */
BOOL BattleEnd_HasCoinReward(void) {
  if (gLinkPlay != 0) {
    return TRUE;
  }
  if (data_027c027c->unk_48[data_027c0284]->gameMode != GAME_MODE_VERSUS) {
    return Player1Team_HasPlayerInGame() ? TRUE : FALSE;
  }
  return TRUE;
}
