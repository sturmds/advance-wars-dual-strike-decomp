#include "types.h"
#include "game/Unk_02080440.h"

BOOL Ov006_IsRoadOrVerticalBridge(
    int x, int y); /* road, or a bridge with cell MAP_CELL_BRIDGE_VERTICAL */
BOOL Ov006_IsRoadOrHorizontalBridge(
    int x, int y); /* road, or a bridge with cell MAP_CELL_BRIDGE_HORIZONTAL */
BOOL Ov006_IsRoadOrBridgeTile(int x, int y); /* terrain road or bridge */

char data_ov006_0236c0d4[] = {
#include "data/ov006/data_ov006_0236c0d4.inc.c"
};
char data_ov006_0236c0d8[] = {
#include "data/ov006/data_ov006_0236c0d8.inc.c"
};
static s16 Ov006_gRoadCellByMask[] = {
#include "data/ov006/Ov006_gRoadCellByMask.inc.c"
};
s16 Ov006_gShoalGraphicTable[] = {
#include "data/ov006/Ov006_gShoalGraphicTable.inc.c"
};
s16 Ov006_gWaterCellTable[] = {
#include "data/ov006/Ov006_gWaterCellTable.inc.c"
};
/* indexed by a 3x3 neighbour mask, bit 8 top left to bit 0 bottom right */
s16 data_ov006_0236cadc[] = {
#include "data/ov006/data_ov006_0236cadc.inc.c"
};
/* indexed by a 3x3 neighbour mask, bit 8 top left to bit 0 bottom right */
s16 data_ov006_0236cedc[] = {
#include "data/ov006/data_ov006_0236cedc.inc.c"
};

/* Picks the road cell for x, y from its neighbours. Each bit of the mask
 * says a neighbour joins the road: 7 above, 6 above right, 5 left, 4 the
 * tile itself, 3 right, 2 below left, 1 below, 0 below right. The tile above
 * left is not looked at. Off-map neighbours count as not joined. */
int Ov006_GetRoadCell(int x, int y) {
  int mask = 0;

  if (y > 0) {
    mask |= Ov006_IsRoadOrVerticalBridge(x, y - 1) << 7;
    if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsRoadOrBridgeTile(x + 1, y - 1) << 6;
    }
  }
  if (x > 0) {
    mask |= Ov006_IsRoadOrHorizontalBridge(x - 1, y) << 5;
  }
  mask |= Ov006_IsRoadOrBridgeTile(x, y) << 4;
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    mask |= Ov006_IsRoadOrHorizontalBridge(x + 1, y) << 3;
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    if (x > 0) {
      mask |= Ov006_IsRoadOrBridgeTile(x - 1, y + 1) << 2;
    }
    mask |= Ov006_IsRoadOrVerticalBridge(x, y + 1) << 1;
    if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
      mask |= Ov006_IsRoadOrBridgeTile(x + 1, y + 1);
    }
  }
  return Ov006_gRoadCellByMask[mask];
}
