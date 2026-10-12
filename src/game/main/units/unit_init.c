#include "types.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

/* Sets up a new unit of the given type: full hp, the type's full fuel and
 * ammo, no cargo, no capture progress and no AI target. Unit_Create and the
 * unit info panel call it. */
void Unit_Init(MapUnit* unit, u8 type) {
  unit->type = type;
  unit->flags = 0;
  unit->hp = 100;
  unit->fuel = Ov000_gUnitStats[type].fuel;
  unit->ammo = Ov000_gUnitStats[type].ammo;
  unit->captureProgress = 0;
  unit->cargo[0] = 0;
  unit->cargo[1] = 0;
  unit->aiOrder = 0;
  unit->aiTargetX = MAP_UNIT_NO_TARGET;
  unit->aiTargetY = MAP_UNIT_NO_TARGET;
  unit->unk_0c_4 = 1;
  unit->serviceWanted = 0;
  unit->transportWanted = 0;
  unit->unk_08_7 = 0;
  unit->unk_0c_5 = 0;
  unit->unk_0c_6 = 0;
  unit->claimedLandmass = 0;
}
