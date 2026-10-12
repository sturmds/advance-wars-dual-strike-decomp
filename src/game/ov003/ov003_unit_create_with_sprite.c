#include "types.h"
#include "game/ov003_unit.h"

Ov003Unit* Ov003Unit_Create(int slot, int column, int x, int y);
void Ov003_CreateUnitSprite(int x, int y, Ov003Unit* unit);

/* Creates a unit; when it gets one, makes it blink and take no damage for 180 frames and
 * gives it a sprite. Returns FALSE when no unit was created. */
BOOL Ov003Unit_CreateWithSprite(int slot, int column, int x, int y) {
  Ov003Unit* unit = Ov003Unit_Create(slot, column, x, y);

  if (unit == NULL) {
    return FALSE;
  }
  unit->unk_17_6 = 1;
  unit->safeTimer = 180;
  Ov003_CreateUnitSprite(unit->x >> 3, unit->y >> 3, unit);
  return TRUE;
}
