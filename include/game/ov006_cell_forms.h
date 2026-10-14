#ifndef GAME_OV006_CELL_FORMS_H
#define GAME_OV006_CELL_FORMS_H

#include "types.h"

/* Two lists of 49 map cell codes, entry by entry the same cell in two
 * forms: row 0 of Ov006_gCellsOpen holds the form for a tile with open
 * ground to its left, row 1 the form beside a mountain, wood or building.
 * The last 25 are the same in both. Ov006_gCellsBesideTall names row 1 (a
 * linker alias): Ov006_MatchCellToLeftTile loads both addresses from its
 * literal pool. */
#define CELL_PAIR_COUNT 49
/* The sizes are written as numbers because tools/data.py does not expand
 * macros in a declaration. */
extern s16 Ov006_gCellsOpen[2][49];
extern s16 Ov006_gCellsBesideTall[49];

#endif
