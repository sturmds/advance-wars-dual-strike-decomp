#ifndef GAME_MAP_UNIT_H
#define GAME_MAP_UNIT_H

#include "types.h"

/* MapUnit.flags. From romhacking.net doc 890 (RadioTails); only the bits
 * named in code comments have been seen used. */
#define MAP_UNIT_WAITED    0x01
#define MAP_UNIT_SELECTED  0x02
#define MAP_UNIT_LOCKED    0x04
#define MAP_UNIT_CARRIED   0x08
#define MAP_UNIT_CARRYING  0x10
#define MAP_UNIT_DIVED     0x20
#define MAP_UNIT_PARALYSED 0x40

/* Unit slots per player in Unk_02080440_48.units. */
#define MAP_UNITS_PER_PLAYER 64

/* MapUnit.aiTargetX and aiTargetY when the unit has no target. */
#define MAP_UNIT_NO_TARGET 63

/* A unit on the map, 0xe bytes. */
typedef struct MapUnit {
    u8 type;                           /* enum UnitType */
    u8 flags;                          /* MAP_UNIT_* */
    u8 x;
    u8 y;
    u8 cargo[2];                       /* slot of each carried unit */
    u16 hp : 7;                        /* 100 = full */
    u16 ammo : 4;
    u16 unk_06_11 : 5;
    u8 fuel : 7;
    u8 unk_08_7 : 1;
    u8 pad_09;
    u16 aiTargetX : 6;                 /* MAP_UNIT_NO_TARGET when unused */
    u16 aiTargetY : 6;
    u16 unk_0a_12 : 2;
    u16 unk_0a_14 : 2;
    u8 aiOrder : 4;                    /* 8, 9 and 10 use aiTargetX and aiTargetY */
    u8 unk_0c_4 : 1;
    u8 unk_0c_5 : 1;
    u8 unk_0c_6 : 1;
    u8 unk_0c_7 : 1;
    u8 pad_0d;
} MapUnit;

typedef char MapUnit_size_check[sizeof(MapUnit) == 0xe ? 1 : -1];

#endif
