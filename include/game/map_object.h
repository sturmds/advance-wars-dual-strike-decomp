#ifndef GAME_MAP_OBJECT_H
#define GAME_MAP_OBJECT_H

#include "types.h"

/* A large object on the map that units can attack (kinds 0-13, for
 * Combat_CalcDamageToTarget), found by tile with func_020db148. Size
 * unknown. */
typedef struct MapObject {
    u8 x;
    u8 y;
    u16 unk_02_0 : 6;
    u16 kind : 4;                      /* damage chart target kind */
    u16 unk_02_10 : 6;
    u8 hp;                             /* 0: destroyed (func_020db148 then returns NULL) */
} MapObject;

#endif
