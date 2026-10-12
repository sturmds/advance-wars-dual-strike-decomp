#ifndef GAME_UNITDROPTASK_H
#define GAME_UNITDROPTASK_H

#include "types.h"

/*
 * Task that runs gUnitDropScript for Sensei's powers. Its script step
 * UnitDrop_SpawnUnit places a unit of type unitType at each chosen tile.
 */
typedef struct UnitDropTask {
  u8 pad_00[0x2c];
  int x; /* tile the search for a drop site has reached */
  int y;
  u8 pad_34[0x54 - 0x34];
  int unitType; /* enum UnitType */
} UnitDropTask;

#endif
