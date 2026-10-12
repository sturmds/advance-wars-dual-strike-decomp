#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counts the current player's direct and indirect ground vehicles (tires and
 * tread; slots 1-50 only). TRUE when there are enough indirects: at least as
 * many as directs when the front CO gives Artillery extra range and flags is
 * 0, otherwise at least half as many. The build step skips indirect types
 * while this is TRUE. flags comes from the caller; its meaning is unknown. */
BOOL AiBuild_HasEnoughGroundIndirects(int flags) {
  int direct = 0;
  int indirect = 0;
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (Ov000_gUnitStats[unit->type].moveGroup &
                                    (MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
      if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_DIRECT) {
        direct++;
      }
      if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_INDIRECT) {
        indirect++;
      }
    }
  }
  if (flags == 0 &&
      GetCoRangeBonus(STATE->currentPlayer, 0, 0, UNIT_ARTILLERY) > 0) {
    if (indirect == 0) {
      return FALSE;
    }
    if (direct > indirect) {
      return FALSE;
    }
  } else if (direct > indirect * 2) {
    return FALSE;
  }
  return TRUE;
}
