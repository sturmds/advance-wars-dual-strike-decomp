#include "types.h"
#include "game/Unk_02080440.h"

extern u8 gCycleHookSteps[];

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* A 16-step cycle of 4 frames each: step value 1 or 2 calls the screen config's
 * hook with 1 or 0 (the second argument is always 8); 0 does nothing. */
void ScreenConfig_RunCycleHook(int frame) {
  switch (gCycleHookSteps[(frame >> 2) & 15]) {
    case 0:
      return;
    case 1:
      if (HOOKS->unk_24 != NULL) {
        HOOKS->unk_24(1, 8);
      }
      break;
    case 2:
      if (HOOKS->unk_24 != NULL) {
        HOOKS->unk_24(0, 8);
      }
      break;
  }
}
