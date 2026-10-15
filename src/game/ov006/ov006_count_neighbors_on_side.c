#include "types.h"
#include "game/Unk_02080440.h"

int Ov006_IsLandNeighbor(int x, int y);

/* Values of side; 3 counts nothing. */
#define SIDE_LEFT 0
#define SIDE_RIGHT 1
#define SIDE_TOP 2
#define SIDE_BOTTOM 4

/* Counts the tiles Ov006_IsLandNeighbor accepts among the three on one side of
 * x, y (corners included) and the two next to x, y on the other axis (above
 * and below for left and right). Tiles off the map count as 0. */
int Ov006_CountNeighborsOnSide(int x, int y, int side) {
  int count = 0;

  switch (side) {
    case SIDE_LEFT:
      if (x > 0) {
        if (y > 0) {
          count += Ov006_IsLandNeighbor(x - 1, y - 1);
        }
        count += Ov006_IsLandNeighbor(x - 1, y);
        if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
          count += Ov006_IsLandNeighbor(x - 1, y + 1);
        }
      }
      if (y > 0) {
        count += Ov006_IsLandNeighbor(x, y - 1);
      }
      if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
        count += Ov006_IsLandNeighbor(x, y + 1);
      }
      break;
    case SIDE_RIGHT:
      if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
        if (y > 0) {
          count += Ov006_IsLandNeighbor(x + 1, y - 1);
        }
        count += Ov006_IsLandNeighbor(x + 1, y);
        if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
          count += Ov006_IsLandNeighbor(x + 1, y + 1);
        }
      }
      if (y > 0) {
        count += Ov006_IsLandNeighbor(x, y - 1);
      }
      if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
        count += Ov006_IsLandNeighbor(x, y + 1);
      }
      break;
    case SIDE_TOP:
      if (y > 0) {
        if (x > 0) {
          count += Ov006_IsLandNeighbor(x - 1, y - 1);
        }
        count += Ov006_IsLandNeighbor(x, y - 1);
        if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
          count += Ov006_IsLandNeighbor(x + 1, y - 1);
        }
      }
      if (x > 0) {
        count += Ov006_IsLandNeighbor(x - 1, y);
      }
      if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
        count += Ov006_IsLandNeighbor(x + 1, y);
      }
      break;
    case SIDE_BOTTOM:
      if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
        if (x > 0) {
          count += Ov006_IsLandNeighbor(x - 1, y + 1);
        }
        count += Ov006_IsLandNeighbor(x, y + 1);
        if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
          count += Ov006_IsLandNeighbor(x + 1, y + 1);
        }
      }
      if (x > 0) {
        count += Ov006_IsLandNeighbor(x - 1, y);
      }
      if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
        count += Ov006_IsLandNeighbor(x + 1, y);
      }
      break;
  }
  return count;
}
