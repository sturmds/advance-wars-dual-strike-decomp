#include "types.h"
#include "game/unit_stats.h"

/* Damage chart column for a target: its unit type, or the dived column
 * when a Stealth or Sub is hidden. */
int DamageChart_GetColumn(int targetType, BOOL targetDived) {
  if (!targetDived) {
    return targetType;
  }
  switch (targetType) {
    case UNIT_STEALTH:
      return DAMAGE_COLUMN_DIVED_STEALTH;
    case UNIT_SUB:
      return DAMAGE_COLUMN_DIVED_SUB;
  }
  return targetType;
}
