#include "types.h"
#include "game/unlocks.h"

/* Clears the seen flags 54 and 55 unless flag 56 is set. */
void CombatIntro_ResetSeenFlags(void) {
  if (GameFlag_IsSet(56)) {
    return;
  }
  GameFlag_Set(54, 0);
  GameFlag_Set(55, 0);
}
