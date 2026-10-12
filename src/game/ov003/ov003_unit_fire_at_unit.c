#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];

void Ov003Unit_SpawnEffect(Ov003Unit* unit, int dx, int dy);

/* Fires at the unit with the given index; the offset is passed in tiles of 8. */
void Ov003Unit_FireAtUnit(Ov003Unit* unit, int targetIndex) {
  Ov003Unit* other = &data_ov003_02369538[targetIndex];

  Ov003Unit_SpawnEffect(unit, (other->x - unit->x) >> 3,
                        (other->y - unit->y) >> 3);
}
