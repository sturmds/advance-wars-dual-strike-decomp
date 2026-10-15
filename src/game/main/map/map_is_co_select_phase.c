#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE while coSelectPhase is 1. func_020e8970 sets it to 1 when
 * func_020e7f30 holds (a map where a player picks a CO). While it is set,
 * Player_PlayTheme plays the CO Select song instead of battle music,
 * VisionIncrease leaves out Scout and Eagle Eye, and Map_InitBattle leaves
 * out some setup. The name is a guess from those uses. */
BOOL Map_IsCoSelectPhase(void) {
  if (data_027c027c->unk_48[data_027c0284]->coSelectPhase == 1) {
    return TRUE;
  }
  return FALSE;
}
