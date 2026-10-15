#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/ai.h"

/* Capture value at which a property falls. */
#define CAPTURE_GOAL 20

/* TRUE when the unit's capture progress plus this turn's points reach the
 * goal. A unit whose owner slot differs from currentUnitBase and that has
 * no progress yet gets FALSE. The code compares a slot (1-4) with a slot
 * base (0, 64, ...), which looks odd; kept as the ROM has it. */
int Unit_CaptureCompletes(MapUnit* unit) {
  if (MAP_UNIT_OWNER(MAP_STATE, unit) != MAP_STATE->currentUnitBase) {
    if (unit->captureProgress == 0) {
      return FALSE;
    }
  }
  return Unit_CapturePoints(unit) + unit->captureProgress >= CAPTURE_GOAL;
}
