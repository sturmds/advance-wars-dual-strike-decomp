#include "types.h"
#include "game/Unk_02080440.h"

s32 HwMod32(s32 numerator, s32 denominator);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE on every sixth day (6, 12, ...), using Unk_02080440_48.unk_00, which
 * AiTurn_RunEndActions compares with 1 and is likely the day. */
BOOL func_ov001_02351cc8(void) {
  if (STATE->day == 0) {
    return FALSE;
  }
  return HwMod32(STATE->day, 6) == 0;
}
