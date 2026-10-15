#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/predeployed_unit.h"
#include "game/unit_stats.h"

/* Puts a unit of the given type for the current player at (x, y). */
MapUnit* Unit_Create(int x, int y, int type);
void func_020c8888(void);

/* Places a map's pre-deployed unit list, army by army, then restores the
 * current player. Fuel and ammo above the unit type's maximum are capped. */
void Map_PlacePredeployedUnits(const PredeployedUnit* row) {
  u16 savedPlayer = MAP_STATE->currentPlayer;
  u8 count = 0;
  u8 slots
      [56]; /* unit slot of each row in this army; length from the stack frame */
  MapUnit* unit;

  for (;;) {
    if (row->x == PREDEPLOYED_END) {
      break;
    }
    if (row->x == PREDEPLOYED_ARMY) {
      MAP_STATE->currentPlayer = row->y;
      count = 0;
      row++;
      MAP_STATE->currentUnitBase =
          (MAP_STATE->currentPlayer - 1) * MAP_UNITS_PER_PLAYER;
      continue;
    }
    unit = Unit_Create(row->x, row->y, row->type);
    if (unit == NULL) {
      break;
    }
    slots[count++] = unit - MAP_STATE->units;
    unit->hp = row->hp;
    unit->fuel = row->fuel;
    if (row->ammo == PREDEPLOYED_AMMO_FULL) {
      /* the most the field holds; capped to the type's ammo below */
      unit->ammo = 15;
    } else {
      unit->ammo = row->ammo;
    }
    unit->flags = row->flags;
    unit->aiOrder = row->aiOrder;
    switch (unit->aiOrder) {
      case 8:
      case 9:
      case 10:
        unit->aiTargetX = row->aiTargetX;
        unit->aiTargetY = row->aiTargetY;
        break;
      default:
        unit->aiTargetX = MAP_UNIT_NO_TARGET;
        unit->aiTargetY = MAP_UNIT_NO_TARGET;
        break;
    }
    unit->unk_0c_4 = row->unk_0a;
    unit->serviceWanted = 0;
    unit->transportWanted = 0;
    unit->unk_08_7 = 0;
    unit->unk_0c_5 = 0;
    unit->unk_0c_6 = 0;
    unit->claimedLandmass = 0;
    if (row->cargo[0] != 0) {
      unit->cargo[0] = slots[row->cargo[0] - 1];
    }
    if (row->cargo[1] != 0) {
      unit->cargo[1] = slots[row->cargo[1] - 1];
    }
    row++;
    if (unit->fuel > Ov000_gUnitStats[unit->type].fuel) {
      unit->fuel = Ov000_gUnitStats[unit->type].fuel;
    }
    if (unit->ammo > Ov000_gUnitStats[unit->type].ammo) {
      unit->ammo = Ov000_gUnitStats[unit->type].ammo;
    }
  }
  MAP_STATE->currentPlayer = savedPlayer;
  func_020c8888();
  MAP_STATE->currentUnitBase =
      (MAP_STATE->currentPlayer - 1) * MAP_UNITS_PER_PLAYER;
}
