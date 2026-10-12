#include "types.h"
#include "game/ai_profile.h"
#include "game/unit_stats.h"

extern s8 data_ov000_022b3850
    [UNIT_TYPE_COUNT]; /* unit info panel entry per type; -1: none */

/* True when the type has an info panel entry, is not a Piperunner or Oozium
 * (never allowed here), and AiProfile.units[].unk_00 is set for it. */
BOOL AiProfile_AllowsBuild(u32 unitType) {
  if (unitType >= UNIT_TYPE_COUNT) {
    return FALSE;
  }
  if (data_ov000_022b3850[unitType] == -1) {
    return FALSE;
  }
  if (unitType == UNIT_PIPERUNNER) {
    return FALSE;
  }
  if (unitType == UNIT_OOZIUM) {
    return FALSE;
  }
  return gAiProfile->units[unitType - 1].unk_00 != 0;
}
