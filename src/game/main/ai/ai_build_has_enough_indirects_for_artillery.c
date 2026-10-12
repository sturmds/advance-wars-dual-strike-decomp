#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Artillery counterpart of AiBuild_HasEnoughGroundIndirects, with the same
 * count: TRUE while the current player has at least one indirect ground
 * vehicle per three direct ones. When flags is 0 and the front CO gives
 * Artillery extra range, one more direct is counted, so Artillery gets built
 * a little sooner. */
BOOL AiBuild_HasEnoughIndirectsForArtillery(int flags) {
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
    direct++;
  }
  return direct <= indirect * 3;
}
