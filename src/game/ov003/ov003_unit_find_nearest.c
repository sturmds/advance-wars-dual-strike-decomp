#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];

u32 Math_Distance(int x1, int y1, int x2, int y2);
int func_ov003_023622b8(void);

/* Returns the distance (in 8-pixel steps) to the nearest entry in the
 * unit table that is in use and has unk_17_6 clear, and stores its index in
 * *nearestIndex (-1 when there is none; the distance is then 0xffffffff). */
u32 Ov003Unit_FindNearest(Ov003Unit* unit, s8* nearestIndex) {
  Ov003Unit* other = data_ov003_02369538;
  u32 best = -1;
  int i;

  *nearestIndex = -1;
  for (i = 0; i < func_ov003_023622b8(); i++, other++) {
    if (other->hp != 0 && !other->unk_17_6) {
      u32 distance = Math_Distance(unit->x >> 3, unit->y >> 3, other->x >> 3,
                                   other->y >> 3);

      if (distance <= best) {
        best = distance;
        *nearestIndex = other - data_ov003_02369538;
      }
    }
  }
  return best;
}
