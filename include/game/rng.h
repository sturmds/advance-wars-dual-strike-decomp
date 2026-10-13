#ifndef GAME_RNG_H
#define GAME_RNG_H

#include "types.h"

/* State of the random number generator that Rng_Next steps. Battle saves
 * and unit actions keep a copy (BattleSaveRecord.rngState, UnitAction.rngState). */
extern u32 gRngState;

#endif
