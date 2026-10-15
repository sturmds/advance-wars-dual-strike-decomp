#include "types.h"
#include "game/Unk_021694c0.h"
#include "game/screen.h"

extern Unk_021694c0* data_021694c0;

int func_020f225c(void);
BOOL TimeLimit_GetRemaining(int* value);

/* Per-frame step while the effect scene is in mode 2: when func_020f225c is
 * nonzero and a time limit is running, counts unk_3c up to unk_38. */
void EffectScene_TickProgress(void) {
  int remaining;

  if (Scene_GetKind() != 2) {
    return;
  }
  if (func_020f225c() == 0) {
    return;
  }
  if (!TimeLimit_GetRemaining(&remaining)) {
    return;
  }
  if (data_021694c0->unk_3c < data_021694c0->unk_38) {
    data_021694c0->unk_3c++;
  }
}
