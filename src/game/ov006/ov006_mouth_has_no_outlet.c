#include "types.h"
#include "game/Unk_02080440.h"

BOOL Ov006_IsLandNeighbor(
    int x,
    int y); /* guess: TRUE on water a river can run into (sea, shoal, reef, river mouth) */

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

/* River mouth cell codes; each names the side of the river it sits on. */
#define CELL_MOUTH_UP 0xfc
#define CELL_MOUTH_RIGHT 0xfd
#define CELL_MOUTH_DOWN 0x11c
#define CELL_MOUTH_LEFT 0x11d

/* Map editor: for a river mouth at x, y, looks at the tile on its open side,
 * then at the tiles on either side of that one, stopping at the first that
 * Ov006_IsLandNeighbor accepts. TRUE when none does, and for any other cell. */
BOOL Ov006_MouthHasNoOutlet(int x, int y) {
  int found = 0;

  switch (CELL_AT(x, y)) {
    case CELL_MOUTH_UP:
      if (y > 0) {
        if (Ov006_IsLandNeighbor(x, y - 1)) {
          found++;
        }
        if (found == 0 && x > 0) {
          if (Ov006_IsLandNeighbor(x - 1, y - 1)) {
            found++;
          } else if (Ov006_IsLandNeighbor(x - 1, y)) {
            found++;
          }
        }
        if (found == 0 && x < STATE->unk_1c4->mapWidth - 1) {
          if (Ov006_IsLandNeighbor(x + 1, y - 1)) {
            found++;
          } else if (Ov006_IsLandNeighbor(x + 1, y)) {
            found++;
          }
        }
      }
      break;
    case CELL_MOUTH_RIGHT:
      if (x < STATE->unk_1c4->mapWidth - 1) {
        if (Ov006_IsLandNeighbor(x + 1, y)) {
          found++;
        }
        if (found == 0 && y > 0) {
          if (Ov006_IsLandNeighbor(x + 1, y - 1)) {
            found++;
          } else if (Ov006_IsLandNeighbor(x, y - 1)) {
            found++;
          }
        }
        if (found == 0 && y < STATE->unk_1c4->mapHeight - 1) {
          if (Ov006_IsLandNeighbor(x + 1, y + 1)) {
            found++;
          } else if (Ov006_IsLandNeighbor(x, y + 1)) {
            found++;
          }
        }
      }
      break;
    case CELL_MOUTH_DOWN:
      if (y < STATE->unk_1c4->mapHeight - 1) {
        if (Ov006_IsLandNeighbor(x, y + 1)) {
          found++;
        }
        if (found == 0 && x > 0) {
          if (Ov006_IsLandNeighbor(x - 1, y + 1)) {
            found++;
          } else if (Ov006_IsLandNeighbor(x - 1, y)) {
            found++;
          }
        }
        if (found == 0 && x < STATE->unk_1c4->mapWidth - 1) {
          if (Ov006_IsLandNeighbor(x + 1, y + 1)) {
            found++;
          } else if (Ov006_IsLandNeighbor(x + 1, y)) {
            found++;
          }
        }
      }
      break;
    case CELL_MOUTH_LEFT:
      if (x > 0) {
        if (Ov006_IsLandNeighbor(x - 1, y)) {
          found++;
        }
        if (found == 0 && y > 0) {
          if (Ov006_IsLandNeighbor(x - 1, y - 1)) {
            found++;
          } else if (Ov006_IsLandNeighbor(x, y - 1)) {
            found++;
          }
        }
        if (found == 0 && y < STATE->unk_1c4->mapHeight - 1) {
          if (Ov006_IsLandNeighbor(x - 1, y + 1)) {
            found++;
          } else if (Ov006_IsLandNeighbor(x, y + 1)) {
            found++;
          }
        }
      }
      break;
  }
  return found == 0;
}
