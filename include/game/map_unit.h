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

/* A unit on the map, 0xe bytes. The fields past flags are not known yet. */
typedef struct MapUnit {
    u8 type;                           /* enum UnitType */
    u8 flags;                          /* MAP_UNIT_* */
    u8 pad_02[0xe - 0x02];
} MapUnit;

#endif
