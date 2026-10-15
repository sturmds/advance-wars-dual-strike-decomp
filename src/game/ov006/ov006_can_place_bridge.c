#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

int Ov006_GetNeighborMask(int x, int y);
int func_ov006_02360308(int x, int y);
int Ov006_GetAdjacentBridgeCell(int x, int y);
int Ov006_IsPlainRiver(int x, int y);

/* TRUE when tile x, y can hold a bridge: it already is one; it is sea, shoal
 * or reef with a fitting set of neighbours (Ov006_GetNeighborMask); or it is
 * a river tile that Ov006_IsPlainRiver accepts, with a neighbour or a bridge
 * on either side across the river's flow (func_ov006_02360308 gives the
 * flow). The inner switches that return TRUE either way are needed to
 * match. */
int Ov006_CanPlaceBridge(int x, int y) {
  int terrain = MAP_TILE_AT(x, y);
  int cell;

  if (terrain == TERRAIN_BRIDGE) {
    return TRUE;
  }
  if (terrain == TERRAIN_SEA || terrain == TERRAIN_SHOAL ||
      terrain == TERRAIN_REEF) {
    switch (Ov006_GetNeighborMask(x, y)) {
      case DESIGN_NB_DOWN:
      case DESIGN_NB_UP:
        switch (func_ov006_02360308(x, y)) {
          case DESIGN_NB_RIGHT:
          case DESIGN_NB_LEFT:
          case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
            return TRUE;
        }
      case DESIGN_NB_UP | DESIGN_NB_DOWN:
        return TRUE;
      case DESIGN_NB_RIGHT:
      case DESIGN_NB_LEFT:
        switch (func_ov006_02360308(x, y)) {
          case DESIGN_NB_DOWN:
          case DESIGN_NB_UP:
          case DESIGN_NB_UP | DESIGN_NB_DOWN:
            return TRUE;
        }
      case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
        return TRUE;
      case DESIGN_NB_UP | DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
      case DESIGN_NB_UP | DESIGN_NB_LEFT | DESIGN_NB_DOWN:
        return TRUE;
      case DESIGN_NB_LEFT | DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
      case DESIGN_NB_UP | DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
        return TRUE;
      case 0:
      default:
        if (Ov006_GetAdjacentBridgeCell(x, y) > 0) {
          return TRUE;
        }
      case DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
      case DESIGN_NB_LEFT | DESIGN_NB_DOWN:
      case DESIGN_NB_UP | DESIGN_NB_RIGHT:
      case DESIGN_NB_UP | DESIGN_NB_LEFT:
        return FALSE;
    }
  }
  if (Ov006_IsPlainRiver(x, y) == 0) {
    return FALSE;
  }
  cell = 0;
  switch (func_ov006_02360308(x, y)) {
    case 0:
    case DESIGN_NB_UP:
    case DESIGN_NB_UP | DESIGN_NB_DOWN:
      if (Ov006_GetNeighborMask(x, y) & (DESIGN_NB_LEFT | DESIGN_NB_RIGHT)) {
        cell = MAP_CELL_BRIDGE_HORIZONTAL;
      } else if ((x > 0 && MAP_TILE_AT(x - 1, y) == TERRAIN_BRIDGE) ||
                 (x < MAP_STATE->unk_1c4->mapWidth - 1 &&
                  MAP_TILE_AT(x + 1, y) == TERRAIN_BRIDGE)) {
        cell = MAP_CELL_BRIDGE_HORIZONTAL;
      }
      break;
    case DESIGN_NB_RIGHT:
    case DESIGN_NB_LEFT:
    case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
      if (Ov006_GetNeighborMask(x, y) & (DESIGN_NB_UP | DESIGN_NB_DOWN)) {
        cell = MAP_CELL_BRIDGE_VERTICAL;
      } else if ((y > 0 && MAP_TILE_AT(x, y - 1) == TERRAIN_BRIDGE) ||
                 (y < MAP_STATE->unk_1c4->mapHeight - 1 &&
                  MAP_TILE_AT(x, y + 1) == TERRAIN_BRIDGE)) {
        cell = MAP_CELL_BRIDGE_VERTICAL;
      }
      break;
  }
  return cell > 0;
}
