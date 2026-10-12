#include "types.h"
#include "game/design_map_gen.h"

s32 Rng_Range(s32 min, s32 max);

/* Scratch grid of the map generator, [y][x], 30 columns. */
#define GRID Ov006_gGenMarks

/* Marks a three-segment path from (x1, y1) to (x2, y2) with 2 in the
 * scratch grid (Ov006_PlaceRoadsAtMarks later turns those cells into cell code
 * 97). Without vertical: a random column between the two x, rows y1 and y2
 * run to it, and the column joins them. With vertical: the same with a random
 * row. The caller takes the endpoints and vertical from Ov006_PickLinkTiles. */
void Ov006_MarkLinkPath(int x1, int y1, int x2, int y2, int vertical) {
  int mid;
  int i;

  if (vertical == 0) {
    if (x1 < x2) {
      mid = Rng_Range(x1, x2);
    } else {
      mid = Rng_Range(x2, x1);
    }
    if (mid < x1) {
      for (i = mid; i <= x1; i++) {
        GRID[y1][i] = 2;
      }
    } else {
      for (i = x1; i <= mid; i++) {
        GRID[y1][i] = 2;
      }
    }
    if (mid < x2) {
      for (i = mid; i <= x2; i++) {
        GRID[y2][i] = 2;
      }
    } else {
      for (i = x2; i <= mid; i++) {
        GRID[y2][i] = 2;
      }
    }
    if (y1 < y2) {
      for (i = y1; i <= y2; i++) {
        GRID[i][mid] = 2;
      }
    } else {
      for (i = y2; i <= y1; i++) {
        GRID[i][mid] = 2;
      }
    }
  } else {
    if (y1 < y2) {
      mid = Rng_Range(y1, y2);
    } else {
      mid = Rng_Range(y2, y1);
    }
    if (mid < y1) {
      for (i = mid; i <= y1; i++) {
        GRID[i][x1] = 2;
      }
    } else {
      for (i = y1; i <= mid; i++) {
        GRID[i][x1] = 2;
      }
    }
    if (mid < y2) {
      for (i = mid; i <= y2; i++) {
        GRID[i][x2] = 2;
      }
    } else {
      for (i = y2; i <= mid; i++) {
        GRID[i][x2] = 2;
      }
    }
    if (x1 < x2) {
      for (i = x1; i <= x2; i++) {
        GRID[mid][i] = 2;
      }
    } else {
      for (i = x2; i <= x1; i++) {
        GRID[mid][i] = 2;
      }
    }
  }
}
