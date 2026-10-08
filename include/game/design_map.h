#ifndef GAME_DESIGN_MAP_H
#define GAME_DESIGN_MAP_H

#include "types.h"

/* One row of data_022a6c84, 27 bytes. Row n holds map id
 * MAP_DESIGN_FIRST + n. */
typedef struct DesignMap {
    u8 pad_00[0x14];
    u8 armyColours[5];                 /* per slot, like MapHeader +0x88 */
    u8 pad_19[0x1b - 0x19];
} DesignMap;

typedef char DesignMap_size_check[sizeof(DesignMap) == 0x1b ? 1 : -1];

#endif
