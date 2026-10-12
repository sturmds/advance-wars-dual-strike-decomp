#include "types.h"
#include "game/ov003_unit.h"

void Ov003Unit_SpawnFacingEffect(Ov003Unit* unit);

void Ov003Unit_FireForward(Ov003Unit* unit) {
  Ov003Unit_SpawnFacingEffect(unit);
}
