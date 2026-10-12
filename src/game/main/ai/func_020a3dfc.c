#include "types.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

/* Whether a unit type is in the list kept for the terrain of a tile (base,
 * airport or port), 0 for any other terrain. A type that is not in the
 * airport list is checked against the port list. What the lists mean is not
 * known yet, so the name stays. */
int func_020a3dfc(int unitType, int tile) {
  switch (tile & 31) {
    case TERRAIN_BASE:
      switch (unitType) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 14:
        case 17:
        case 19:
        case 21:
          return 1;
      }
      break;
    case TERRAIN_AIRPORT:
      switch (unitType) {
        case 9:
        case 12:
        case 14:
        case 15:
        case 16:
        case 25:
          return 1;
      }
    case TERRAIN_PORT:
      switch (unitType) {
        case 9:
        case 12:
        case 17:
        case 19:
        case 21:
        case 22:
        case 24:
        case 25:
          return 1;
      }
      break;
  }
  return 0;
}
