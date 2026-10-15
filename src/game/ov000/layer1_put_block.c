#include "types.h"
#include "game/Unk_02088d04.h"

void Bg_PutTileBlock(u16* map, int x, int y, const void* block, int tileAdd);
void Gfx_RequestLayerCopy(int layer);

const u32 data_ov000_022da5d4[] = {
#include "data/ov000/data_ov000_022da5d4.inc.c"
};
static const u8 data_ov000_022da5f4[] = {
#include "data/ov000/data_ov000_022da5f4.inc.c"
};
const u8 data_ov000_022da898[] = {
#include "data/ov000/data_ov000_022da898.inc.c"
};
const u8 data_ov000_022dabfc[] = {
#include "data/ov000/data_ov000_022dabfc.inc.c"
};

/* Puts a tile block at the top left of layer 1 and queues the layer for copying. */
void Layer1_PutBlock(void) {
  Bg_PutTileBlock(data_027c0280->layer1Map, 0, 0, data_ov000_022da5f4, 0);
  Gfx_RequestLayerCopy(1);
}
