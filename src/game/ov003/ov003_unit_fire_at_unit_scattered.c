#include "types.h"
#include "game/ov003_unit.h"
#include "game/math.h"

u32 Ov003_RandNext(void);
void Ov003Unit_SpawnEffect(Ov003Unit* unit, u32 dx, u32 dy);

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* Like Ov003Unit_FireAtUnit, but the aim point is off by a random amount
 * of up to the distance to the target (at most 128) on each axis. */
void Ov003Unit_FireAtUnitScattered(Ov003Unit* unit, int targetIndex) {
  Ov003Unit* other = &data_ov003_02369538[targetIndex];
  int spread;
  int offsetX;
  int offsetY;

  spread = ABS(other->x - unit->x);
  if (spread < ABS(other->y - unit->y)) {
    spread = ABS(other->y - unit->y);
  }
  if (spread > 128) {
    spread = 128;
  }
  offsetX = HwMod32(Ov003_RandNext(), spread * 2);
  offsetY = HwMod32(Ov003_RandNext(), spread * 2);
  /* needed to match: the shift is logical, so the offsets are cast to u32 */
  Ov003Unit_SpawnEffect(unit,
                        (u32)(other->x - unit->x + (offsetX - spread)) >> 3,
                        (u32)(other->y - unit->y + (offsetY - spread)) >> 3);
}
