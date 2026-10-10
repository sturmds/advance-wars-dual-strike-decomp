#ifndef GAME_DESIGN_MAP_EDIT_H
#define GAME_DESIGN_MAP_EDIT_H

/* Neighbour bits that the Design Room tile checks return
 * (Ov006_GetNeighborMask, func_ov006_02360308). */
#define DESIGN_NB_DOWN  1
#define DESIGN_NB_RIGHT 2
#define DESIGN_NB_LEFT  4
#define DESIGN_NB_UP    8

/* Map cell codes (MapTiles.mapCells) of the two bridge pieces. */
#define MAP_CELL_BRIDGE_HORIZONTAL 21  /* runs left to right */
#define MAP_CELL_BRIDGE_VERTICAL   22  /* runs top to bottom */

#endif
