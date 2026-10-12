#include "types.h"
#include "game/ai_profile.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Chance in percent (0 to 80) that a foot unit is built as a Mech instead of
 * Infantry: none up to day 3, then profile byte unk_08[2] per day. */
u32 AiTurn_GetMechBuildPercent(void) {
  int chance = 0;

  if (STATE->day > 3) {
    chance = gAiProfile->unk_08[2] * STATE->day;
    if (chance > 80) {
      chance = 80;
    }
  }
  return chance;
}
