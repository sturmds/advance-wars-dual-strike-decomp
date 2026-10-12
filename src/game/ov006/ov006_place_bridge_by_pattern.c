#include "types.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

int Ov006_IsPlainRiver(int x, int y);
int func_ov006_02360308(int x, int y);
int Ov006_GetNeighborMask(int x, int y);
void Ov006_RemoveProperty(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int value);
void Ov006_RefitCellsAround(int x, int y);

/* Puts a bridge on tile x, y: a horizontal piece when func_ov006_02360308
 * reports nothing, up, or up and down, and Ov006_GetNeighborMask has a left
 * or right bit; a vertical piece when it reports right, left, or both and the
 * mask has an up or down bit. Does nothing if Ov006_IsPlainRiver returns 0 for the
 * tile or neither case fits. */
void Ov006_PlaceBridgeByPattern(int x, int y) {
  int cell;

  if (!Ov006_IsPlainRiver(x, y)) {
    return;
  }
  cell = 0;
  switch (func_ov006_02360308(x, y)) {
    case 0:
    case DESIGN_NB_UP:
    case DESIGN_NB_UP | DESIGN_NB_DOWN:
      if (Ov006_GetNeighborMask(x, y) & (DESIGN_NB_LEFT | DESIGN_NB_RIGHT)) {
        cell = MAP_CELL_BRIDGE_HORIZONTAL;
      }
      break;
    case DESIGN_NB_RIGHT:
    case DESIGN_NB_LEFT:
    case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
      if (Ov006_GetNeighborMask(x, y) & (DESIGN_NB_UP | DESIGN_NB_DOWN)) {
        cell = MAP_CELL_BRIDGE_VERTICAL;
      }
      break;
  }
  if (cell > 0) {
    Ov006_RemoveProperty(x, y);
    Ov006_SetTerrain(x, y, TERRAIN_BRIDGE);
    Ov006_StoreMapCell(x, y, cell);
    Ov006_RefitCellsAround(x, y);
  }
}
