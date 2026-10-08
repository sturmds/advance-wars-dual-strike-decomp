#ifndef GAME_UNITDROPTASK_H
#define GAME_UNITDROPTASK_H

#include "types.h"

/*
 * Task that runs gUnitDropScript for Sensei's powers. Its script step
 * func_020e41fc places a unit of type unitType at each chosen tile.
 */
typedef struct UnitDropTask {
    u8 pad_00[0x54];
    int unitType; /* enum UnitType */
} UnitDropTask;

#endif
