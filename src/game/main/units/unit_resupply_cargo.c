#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
/* Ov000_gUnitStats[i].carryTypes through its own symbol. */
extern struct {
  const u8* carryTypes;
  u8 pad_04[sizeof(UnitStats) - 4];
} data_ov000_022f4fd0[];

int Unit_Rearm(MapUnit* unit, BOOL payCost);
int Unit_Refuel(MapUnit* unit, BOOL payCost);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Refills ammo and fuel of every unit aboard a transport without charging
 * anyone. Returns the shown-HP points Unit_Rearm and Unit_Refuel report,
 * added up. carryTypes[0] is the number of cargo slots. The first test
 * reads it through the symbol data_ov000_022f4fd0 and the later ones through
 * Ov000_gUnitStats; needed to match. */
int Unit_ResupplyCargo(MapUnit* transport) {
  u8 i;
  int total = 0;

  i = 0;
  if (i < data_ov000_022f4fd0[transport->type].carryTypes[0]) {
    do {
      if (transport->cargo[i] != 0) {
        total += Unit_Rearm(&STATE->units[transport->cargo[i]], FALSE);
        total += Unit_Refuel(&STATE->units[transport->cargo[i]], FALSE);
      }
      i++;
    } while (i < Ov000_gUnitStats[transport->type].carryTypes[0]);
  }
  return total;
}
