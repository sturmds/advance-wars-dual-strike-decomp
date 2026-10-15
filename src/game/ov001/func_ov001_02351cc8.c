#include "types.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

/* TRUE on every sixth day (6, 12, ...), using Unk_02080440_48.unk_00, which
 * AiTurn_RunEndActions compares with 1 and is likely the day. */
BOOL func_ov001_02351cc8(void) {
  if (MAP_STATE->day == 0) {
    return FALSE;
  }
  return HwMod32(MAP_STATE->day, 6) == 0;
}
