#ifndef GAME_DESIGN_MAP_EDIT_H
#define GAME_DESIGN_MAP_EDIT_H

/* Neighbour bits that the Design Room tile checks return
 * (Ov006_GetNeighborMask, func_ov006_02360308). */
#define DESIGN_NB_DOWN 1
#define DESIGN_NB_RIGHT 2
#define DESIGN_NB_LEFT 4
#define DESIGN_NB_UP 8

/* Map cell codes (MapTiles.mapCells). */
#define MAP_CELL_SEA 42
#define MAP_CELL_WOOD 135
#define MAP_CELL_REEF 360

/* The two bridge pieces. */
#define MAP_CELL_BRIDGE_HORIZONTAL 21 /* runs left to right */
#define MAP_CELL_BRIDGE_VERTICAL 22   /* runs top to bottom */

/* The map cells of pipe pieces, seams included. */
#define MAP_CELL_IS_PIPE(c)                                        \
  ((c) == 0x142 || (c) == 0x143 || (c) == 0x140 || (c) == 0x141 || \
   (c) == 0x160 || (c) == 0x161 || (c) == 0x162 || (c) == 0x163 || \
   (c) == 0x122 || (c) == 0x123 || (c) == 0x121 || (c) == 0x120 || \
   (c) == 0x103 || (c) == 0x102)

/* Pipe end pieces by the side they open to (Ov006_GetPipeEndCell). */
#define MAP_CELL_PIPE_END_LEFT 0x121
#define MAP_CELL_PIPE_END_RIGHT 0x120
#define MAP_CELL_PIPE_END_UP 0x103
#define MAP_CELL_PIPE_END_DOWN 0x102

/* Pipe seams, horizontal and vertical. */
#define MAP_CELL_PIPE_SEAM_H 0x162
#define MAP_CELL_PIPE_SEAM_V 0x163

/* Neighbour directions; Ov006_gDirStepX and Ov006_gDirStepY hold
 * their x and y steps. */
#define DESIGN_DIR_LEFT 0
#define DESIGN_DIR_RIGHT 1
#define DESIGN_DIR_UP 2
#define DESIGN_DIR_DOWN 3

/* What Ov006_GetPipeLink finds on the neighbour tile in one direction. */
#define PIPE_LINK_NONE 0   /* off the map, or not a pipe */
#define PIPE_LINK_CLOSED 1 /* a pipe piece closed on this side */
#define PIPE_LINK_OPEN 2   /* a pipe piece open toward this tile */
#define PIPE_LINK_ACROSS 3 /* a straight or end piece running the other way */

#endif
