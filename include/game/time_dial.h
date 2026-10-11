#ifndef GAME_TIME_DIAL_H
#define GAME_TIME_DIAL_H

#include "types.h"

/* State of the dial TimeDial_Update draws for the time left; TimeDial_Reset
 * clears angle. The rest of the struct is unknown. */
typedef struct TimeDial {
    u8 pad_00[0x54];
    s32 angle;                         /* hand angle, 0x10000 a full turn */
} TimeDial;

#endif
