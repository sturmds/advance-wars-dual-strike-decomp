#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL CanUseTagPower(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Battle menu entry test: 1 hides the entry. Same shape as the Super and CO
 * power entries beside it; this one is for the tag power (handler
 * PowerMenu_ActivateTag). */
s8 PowerMenu_TagHidden(void) {
  if (STATE->unk_5c == 0) {
    return 1;
  }
  return CanUseTagPower(STATE->currentPlayer) == 0;
}
