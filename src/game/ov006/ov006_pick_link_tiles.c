#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Ov006_Abs(int value);
int Ov006_RandomMapCoord(
    int min, int max,
    int isY); /* random min..max inside the map width (or height when isY) */
int Map_GetDistance(int x1, int y1, s16 x2, s16 y2);
s32 Rng_Range(s32 min, s32 max);

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Map cell code 42 is sea (Ov006_FillMapForScreen). */
#define CELL_SEA 42
#define CELL_AT(p) \
  (STATE->mapTiles->mapCells[STATE->mapTiles->rowStart[(p)->y] + (p)->x])

/* Picks a tile near each of two points, (x1, y1) and (x2, y2), for the
 * caller to join: each tile lies within range of its own point, on the side
 * that faces the other point, and is not sea. When the points are close on x
 * (or on y), the tiles spread across that axis freely; *vertical says which
 * case it was (1: the points are apart on y), random when neither or both are
 * close. */
void Ov006_PickLinkTiles(int x1, int y1, int x2, int y2, int range,
                         TilePoint* out1, TilePoint* out2, int* vertical) {
  int dx = x1 - x2;

  if (Ov006_Abs(dx) >= range || Ov006_Abs(y1 - y2) >= range) {
    if (Ov006_Abs(dx) < range) {
      *vertical = 1;
      do {
        out1->x = Ov006_RandomMapCoord(x1 - range, x1 + range, 0);
        if (y1 > y2) {
          out1->y = Ov006_RandomMapCoord(y1 - range, y1, 1);
        } else {
          out1->y = Ov006_RandomMapCoord(y1, y1 + range, 1);
        }
      } while (CELL_AT(out1) == CELL_SEA ||
               Map_GetDistance(out1->x, out1->y, x1, y1) > range);
      do {
        out2->x = Ov006_RandomMapCoord(x2 - range, x2 + range, 0);
        if (y1 > y2) {
          out2->y = Ov006_RandomMapCoord(y2, y2 + range, 1);
        } else {
          out2->y = Ov006_RandomMapCoord(y2 - range, y2, 1);
        }
      } while (CELL_AT(out2) == CELL_SEA ||
               Map_GetDistance(out2->x, out2->y, x2, y2) > range);
      return;
    }
    if (Ov006_Abs(y1 - y2) < range) {
      *vertical = 0;
      do {
        out1->y = Ov006_RandomMapCoord(y1 - range, y1 + range, 1);
        if (x1 > x2) {
          out1->x = Ov006_RandomMapCoord(x1 - range, x1, 0);
        } else {
          out1->x = Ov006_RandomMapCoord(x1, x1 + range, 0);
        }
      } while (CELL_AT(out1) == CELL_SEA ||
               Map_GetDistance(out1->x, out1->y, x1, y1) > range);
      do {
        out2->y = Ov006_RandomMapCoord(y2 - range, y2 + range, 1);
        if (x1 > x2) {
          out2->x = Ov006_RandomMapCoord(x2, x2 + range, 0);
        } else {
          out2->x = Ov006_RandomMapCoord(x2 - range, x2, 0);
        }
      } while (CELL_AT(out2) == CELL_SEA ||
               Map_GetDistance(out2->x, out2->y, x2, y2) > range);
      return;
    }
  }
  *vertical = Rng_Range(0, 1);
  do {
    if (x1 > x2) {
      out1->x = Ov006_RandomMapCoord(x1 - range, x1, 0);
    } else {
      out1->x = Ov006_RandomMapCoord(x1, x1 + range, 0);
    }
    if (y1 > y2) {
      out1->y = Ov006_RandomMapCoord(y1 - range, y1, 1);
    } else {
      out1->y = Ov006_RandomMapCoord(y1, y1 + range, 1);
    }
  } while (CELL_AT(out1) == CELL_SEA ||
           Map_GetDistance(out1->x, out1->y, x1, y1) > range);
  do {
    if (x1 > x2) {
      out2->x = Ov006_RandomMapCoord(x2, x2 + range, 0);
    } else {
      out2->x = Ov006_RandomMapCoord(x2 - range, x2, 0);
    }
    if (y1 > y2) {
      out2->y = Ov006_RandomMapCoord(y2, y2 + range, 1);
    } else {
      out2->y = Ov006_RandomMapCoord(y2 - range, y2, 1);
    }
  } while (CELL_AT(out2) == CELL_SEA ||
           Map_GetDistance(out2->x, out2->y, x2, y2) > range);
}
