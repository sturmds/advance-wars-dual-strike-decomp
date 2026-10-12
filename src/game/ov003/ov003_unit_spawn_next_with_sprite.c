#include "types.h"
#include "game/ov003_unit.h"

Ov003Unit* Ov003Unit_SpawnNext(void);
void Ov003_CreateUnitSprite(int x, int y, Ov003Unit* unit);

/* Spawns the next unit; when it gets one, makes it blink and take no damage for 180 frames
 * and gives it a sprite. Returns FALSE when no unit was spawned. */
BOOL Ov003Unit_SpawnNextWithSprite(void) {
  Ov003Unit* unit = Ov003Unit_SpawnNext();

  if (unit == NULL) {
    return FALSE;
  }
  unit->unk_17_6 = 1;
  unit->safeTimer = 180;
  Ov003_CreateUnitSprite(unit->x >> 3, unit->y >> 3, unit);
  return TRUE;
}
