#include "types.h"
#include "game/ai_profile.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

#define TYPE_AMMO(type) Ov000_gUnitStats[type].ammo
#define TYPE_FUEL(type) Ov000_gUnitStats[type].fuel

s32 HwDiv32(s32 num, s32 den);

/* Sets MapUnit.serviceWanted for a unit that has none: 2 (repairs) when its hp
 * is below the active profile's unk_04, 1 (supplies) when it has no ammo
 * (and uses ammo) or its fuel percent is below the profile's unk_03. */
void AiUnit_ChooseServiceWanted(MapUnit* unit) {
  if (unit->hp < gAiActiveUnitProfile->unk_04) {
    unit->serviceWanted = 2;
    return;
  }
  if (TYPE_AMMO(unit->type) != 0 && unit->ammo == 0) {
    unit->serviceWanted = 1;
    return;
  }
  if ((u8)HwDiv32(unit->fuel * 100, TYPE_FUEL(unit->type)) <
      gAiActiveUnitProfile->unk_03) {
    unit->serviceWanted = 1;
  }
}
