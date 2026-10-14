#include "types.h"
#include "game/Unk_02088d04.h"

u8* Palette_GetBlock(int index, int player);
void TransferQueue_Add(const void* src, void* dst, u32 size);

extern Unk_027c0280* data_027c0280;
extern const u8 data_ov006_0236a4cc[];
u8 data_ov006_0236d644[0x100]; /* size: up to the next symbol */

/* Fills four slots of 8 object tiles each, from tile 0x25e: 6 tiles from
 * the block Palette_GetBlock gives for each id of data_ov006_0236a4cc (from
 * byte 64 on), then 2 tiles from data_ov006_0236d644. */
void Ov006_LoadSlotTiles(void) {
  int i;
  int tile;
  u8* id;
  u8 ids[4];

  ids[0] = data_ov006_0236a4cc[0];
  ids[1] = data_ov006_0236a4cc[1];
  ids[2] = data_ov006_0236a4cc[2];
  ids[3] = data_ov006_0236a4cc[3];
  id = ids;
  tile = 0x25e;
  for (i = 0; i < 4; i++) {
    TransferQueue_Add(Palette_GetBlock(*id, 0) + 64,
                      data_027c0280->objVram + tile * 32, 192);
    TransferQueue_Add(data_ov006_0236d644,
                      data_027c0280->objVram + (tile + 6) * 32, 64);
    tile += 8;
    id++;
  }
}
