#include "types.h"
#include "game/Unk_02080440.h"

BOOL Ov006_IsRoadOrVerticalBridge(
    int x, int y); /* road, or a bridge with cell MAP_CELL_BRIDGE_VERTICAL */
BOOL Ov006_IsRoadOrHorizontalBridge(
    int x, int y); /* road, or a bridge with cell MAP_CELL_BRIDGE_HORIZONTAL */
BOOL Ov006_IsRoadOrBridgeTile(int x, int y); /* terrain road or bridge */

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern s16 Ov006_gRoadCellByMask[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Picks the road cell for x, y from its neighbours. Each bit of the mask
 * says a neighbour joins the road: 7 above, 6 above right, 5 left, 4 the
 * tile itself, 3 right, 2 below left, 1 below, 0 below right. The tile above
 * left is not looked at. Off-map neighbours count as not joined. */
int Ov006_GetRoadCell(int x, int y) {
  int mask = 0;

  if (y > 0) {
    mask |= Ov006_IsRoadOrVerticalBridge(x, y - 1) << 7;
    if (x < STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsRoadOrBridgeTile(x + 1, y - 1) << 6;
    }
  }
  if (x > 0) {
    mask |= Ov006_IsRoadOrHorizontalBridge(x - 1, y) << 5;
  }
  mask |= Ov006_IsRoadOrBridgeTile(x, y) << 4;
  if (x < STATE->unk_1c4->mapWidth - 1) {
    mask |= Ov006_IsRoadOrHorizontalBridge(x + 1, y) << 3;
  }
  if (y < STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      mask |= Ov006_IsRoadOrBridgeTile(x - 1, y + 1) << 2;
    }
    mask |= Ov006_IsRoadOrVerticalBridge(x, y + 1) << 1;
    if (x < STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsRoadOrBridgeTile(x + 1, y + 1);
    }
  }
  return Ov006_gRoadCellByMask[mask];
}
