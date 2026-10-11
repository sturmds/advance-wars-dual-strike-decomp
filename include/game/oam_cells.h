#ifndef GAME_OAM_CELLS_H
#define GAME_OAM_CELLS_H

#include "types.h"

#define OAM_ATTR1_HFLIP 0x1000
#define OAM_ATTR1_FLIPS 0x3000             /* horizontal and vertical flip */

/* One OBJ of a sprite: OAM attributes 0-2, with the position relative to
 * the sprite and the tile number relative to its first tile. A cell list
 * holds an s16 count, then that many cells; the tables Oam_GetShapeCells
 * returns hold one cell each. */
typedef struct OamCell {
    u16 attr0;                         /* bits 0-7: y */
    u16 attr1;                         /* bits 0-8: x */
    u16 attr2;                         /* bits 0-9: tile */
} OamCell;

#endif
