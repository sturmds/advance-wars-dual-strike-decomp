#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Battle_RegisterFrameCallbacks(void);
void Screen_ResetWindows(void);

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Step of the attack script (data_02168400): runs the screen config's
 * unk_54 hook with 1 when set, registers the battle frame callbacks and
 * resets the windows. */
void AttackTask_SetUpScreen(void) {
  if (HOOKS->unk_54 != NULL) {
    HOOKS->unk_54(1);
  }
  Battle_RegisterFrameCallbacks();
  Screen_ResetWindows();
}
