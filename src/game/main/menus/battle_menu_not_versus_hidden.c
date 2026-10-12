#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Battle menu entry test (table at 0x02165f10): 1 hides the entry in every
 * game mode but Versus. */
s8 BattleMenu_NotVersusHidden(void) {
  return STATE->gameMode != GAME_MODE_VERSUS;
}
