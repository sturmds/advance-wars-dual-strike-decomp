#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_pickup.h"

extern Ov003Unit data_ov003_02369538[];

int func_ov003_023622b8(void);
void Ov003_ApplyPickup(Ov003Unit* unit, Ov003Pickup* pickup);

/* Each frame: the first live unit within 48 of a pickup (not one of the row
 * being spawned into, not one with unk_17_6 set) takes it with
 * Ov003_ApplyPickup; a pickup nobody takes counts its time down. Then each
 * row's slowTimer counts down. Needed to match: the declaration order. */
void Ov003_UpdatePickups(void) {
  Ov003Pickup* pickup;
  Ov003Unit* unit;
  int index;
  int left;
  int right;
  int top;
  int bottom;
  int i;
  pickup = data_ov003_02369b98;

  for (i = 0; i < 4; i++, pickup++) {
    if (pickup->time == 0) {
      continue;
    }
    left = pickup->x - 48;
    right = pickup->x + 48;
    top = pickup->y - 48;
    bottom = pickup->y + 48;
    unit = data_ov003_02369538;
    for (index = 0; index < func_ov003_023622b8(); index++, unit++) {
      if (unit->hp != 0 && unit->unk_17_6 == 0 &&
          unit->row != data_ov003_02364928->row && unit->x + 48 >= left &&
          unit->y + 48 >= top && unit->x - 48 <= right &&
          unit->y - 48 <= bottom) {
        Ov003_ApplyPickup(unit, pickup);
        goto next;
      }
    }
    if (pickup->time != 0) {
      pickup->time--;
    }
  next:;
  }
  for (i = 1; i <= 5; i++) {
    if (data_ov003_02364928->rows[i - 1].slowTimer != 0) {
      data_ov003_02364928->rows[i - 1].slowTimer--;
    }
  }
}
