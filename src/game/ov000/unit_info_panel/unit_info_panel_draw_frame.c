#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
extern u8 data_ov000_022da898[];

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);
void Gfx_RequestLayerCopy(int layer);

/* Puts a tile block (probably the panel's frame) on layer 1 at column 14, row 0
 * of the current screen and queues the layer's copy. */
void UnitInfoPanel_PutFrameBlock(void) {
  Bg_PutTileBlock(data_027c0280->layer1Map, 14, 0, data_ov000_022da898, 0);
  Gfx_RequestLayerCopy(1);
}
