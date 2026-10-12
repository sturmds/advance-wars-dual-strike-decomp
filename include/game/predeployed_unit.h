#ifndef GAME_PREDEPLOYED_UNIT_H
#define GAME_PREDEPLOYED_UNIT_H

#include "types.h"

/* PredeployedUnit.x values that mark special rows. */
#define PREDEPLOYED_ARMY \
  0xfe /* y holds the player number (1-4) of the rows that follow */
#define PREDEPLOYED_END 0xff

/* PredeployedUnit.ammo value that means "full". */
#define PREDEPLOYED_AMMO_FULL 99

/* One row of a map's pre-deployed unit list (MapHeader.units and
 * hardUnits), 13 bytes. */
typedef struct PredeployedUnit {
  u8 x;
  u8 y;
  u8 type;  /* enum UnitType */
  u8 flags; /* MAP_UNIT_* */
  u8 hp;
  u8 ammo;
  u8 fuel;
  u8 cargo
      [2];    /* 1-based row of each carried unit within this army, 0 if none */
  u8 aiOrder; /* MapUnit.aiOrder */
  u8 unk_0a;
  u8 aiTargetX;
  u8 aiTargetY;
} PredeployedUnit;

typedef char PredeployedUnit_size_check[sizeof(PredeployedUnit) == 13 ? 1 : -1];

#endif
