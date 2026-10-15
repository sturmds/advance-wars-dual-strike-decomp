#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

int Ov006_GetNeighborMask(int x, int y);
int func_ov006_02360308(int x, int y);
int Ov006_GetAdjacentBridgeCell(int x, int y);
int Ov006_IsPlainRiver(int x, int y);
void Ov006_RemoveProperty(int x, int y);
void Ov006_TurnToSea(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);
void Ov006_RefitCellsAround(int x, int y);

#define PUT_SEA_BRIDGE(cell)              \
  Ov006_RemoveProperty(x, y);             \
  if (terrain == TERRAIN_SHOAL) {         \
    Ov006_TurnToSea(x, y);                \
  }                                       \
  Ov006_SetTerrain(x, y, TERRAIN_BRIDGE); \
  Ov006_StoreMapCell(x, y, cell);         \
  Ov006_RefitCellsAround(x, y)

/* Puts a bridge on tile x, y where Ov006_CanPlaceBridge allows one, with the
 * piece that fits the neighbours. Sea, shoal and reef tiles: the piece runs
 * between the neighbours Ov006_GetNeighborMask reports, and with only one of
 * them, along the neighbours func_ov006_02360308 reports; a shoal goes
 * through Ov006_TurnToSea first. Other shapes take the cell
 * Ov006_GetAdjacentBridgeCell picks. River tiles: the piece runs across the river's
 * flow. */
void Ov006_PlaceBridge(int x, int y) {
  int terrain = MAP_TILE_AT(x, y);
  int cell;

  if (terrain == TERRAIN_SEA || terrain == TERRAIN_SHOAL ||
      terrain == TERRAIN_REEF) {
    switch (Ov006_GetNeighborMask(x, y)) {
      case DESIGN_NB_DOWN:
      case DESIGN_NB_UP:
        switch (func_ov006_02360308(x, y)) {
          case DESIGN_NB_RIGHT:
          case DESIGN_NB_LEFT:
          case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
            PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_HORIZONTAL);
            return;
        }
      case DESIGN_NB_UP | DESIGN_NB_DOWN:
        PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_VERTICAL);
        return;
      case DESIGN_NB_RIGHT:
      case DESIGN_NB_LEFT:
        switch (func_ov006_02360308(x, y)) {
          case DESIGN_NB_DOWN:
          case DESIGN_NB_UP:
          case DESIGN_NB_UP | DESIGN_NB_DOWN:
            PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_VERTICAL);
            return;
        }
      case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
        PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_HORIZONTAL);
        return;
      case DESIGN_NB_UP | DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
      case DESIGN_NB_UP | DESIGN_NB_LEFT | DESIGN_NB_DOWN:
        PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_VERTICAL);
        return;
      case DESIGN_NB_LEFT | DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
      case DESIGN_NB_UP | DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
        PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_HORIZONTAL);
        return;
      case 0:
      default:
        if (terrain == TERRAIN_SHOAL) {
          Ov006_RemoveProperty(x, y);
          Ov006_TurnToSea(x, y);
        }
        cell = Ov006_GetAdjacentBridgeCell(x, y);
        if (cell > 0) {
          Ov006_RemoveProperty(x, y);
          Ov006_SetTerrain(x, y, TERRAIN_BRIDGE);
          Ov006_StoreMapCell(x, y, cell);
        }
        return;
      case DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
      case DESIGN_NB_LEFT | DESIGN_NB_DOWN:
      case DESIGN_NB_UP | DESIGN_NB_RIGHT:
      case DESIGN_NB_UP | DESIGN_NB_LEFT:
        return;
    }
  }
  if (Ov006_IsPlainRiver(x, y) == 0) {
    return;
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
  if (cell > 0) {
    Ov006_RemoveProperty(x, y);
    Ov006_SetTerrain(x, y, TERRAIN_BRIDGE);
    Ov006_StoreMapCell(x, y, cell);
  }
}
