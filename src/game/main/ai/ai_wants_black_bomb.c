#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Unit_GetValuePerHp(int playerIndex, int unitType);
BOOL AiBuild_WantsBlackBombInstead(int arg);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* FALSE when the current player cannot pay for a Black Bomb; otherwise
 * whatever AiBuild_WantsBlackBombInstead says (it also refuses while an own Black Bomb exists). */
BOOL Ai_WantsBlackBomb(int arg) {
  if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_BLACK_BOMB) * 10) >
      STATE->players[STATE->currentPlayer].funds) {
    return FALSE;
  }
  return AiBuild_WantsBlackBombInstead(arg);
}
