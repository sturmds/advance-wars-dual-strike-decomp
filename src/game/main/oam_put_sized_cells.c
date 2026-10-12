#include "types.h"

const void* Oam_GetShapeCells(int width, int height);
void Oam_PutCells(int attr1, int attr0, const void* cells, int attr2, int tile);

/* Puts the cells for a sprite of the given pixel width and height at (x, y). */
void Oam_PutSizedCells(int x, int y, int width, int height, int attr2,
                       int tile) {
  Oam_PutCells(x, y, Oam_GetShapeCells(width, height), attr2, tile);
}
