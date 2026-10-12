#include "types.h"
#include "game/ov003_unit.h"

Ov003Unit* Ov003Unit_Create(int slot, int column, int x, int y);

BOOL Ov003Unit_TryCreate(int slot, int column, int x, int y) {
  Ov003Unit* unit = Ov003Unit_Create(slot, column, x, y);
  return (unit != NULL) ? TRUE : FALSE;
}
