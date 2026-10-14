#ifndef GAME_GX_QUAD_H
#define GAME_GX_QUAD_H

/* Count of a quad vertex table (data_0215782c and the others the
 * Gx_SendQuad functions read): x, y and z for each of the 4 corners, sent
 * as VTX_16. The externs need the count: without it mwcc reads the table
 * again after each store to the geometry registers. */
#define QUAD_VERTEX_COORDS (4 * 3)

#endif
