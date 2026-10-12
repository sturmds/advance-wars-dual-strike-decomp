#include "types.h"
#include "game/ov003_unit.h"

void Ov003Row_DrawCells(int row);
void Ov003_DrawSlotMarkers(void);
void Gfx_RequestLayerCopy(int layer);

/* Draws each row whose bit is set in activeRows (rows count from 1), then requests a copy of layer 0. */
void Ov003_DrawRows(void) {
  int i;

  for (i = 0; i < data_ov003_02364928->rowCount; i++) {
    if (data_ov003_02364928->activeRows & (1 << i)) {
      Ov003Row_DrawCells(i + 1);
    }
  }
  Ov003_DrawSlotMarkers();
  Gfx_RequestLayerCopy(0);
}
