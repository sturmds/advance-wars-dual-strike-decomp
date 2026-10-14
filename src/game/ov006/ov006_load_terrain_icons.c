#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02080440.h"

const void* Terrain_GetIconTiles(int id);
void TransferQueue_Add(const void* src, void* dst, u32 size);

extern Unk_027c0280* data_027c0280;
s8 data_ov006_0236a7a0[12] = {
#include "data/ov006/data_ov006_0236a7a0.inc.c"
};

/* Queues the 0x100-byte image of each of the 18 entries of data_ov006_0236a7a0
 * (terrain in the low 5 bits) into the object VRAM, 8 tiles apart from tile 0x2ee. */
void Ov006_LoadTerrainIcons(void) {
  int i;

  for (i = 0; i < 18; i++) {
    TransferQueue_Add(
        Terrain_GetIconTiles(MAP_TILE_TERRAIN(data_ov006_0236a7a0[i])),
        data_027c0280->objVram + (0x2ee + i * 8) * 32, 0x100);
  }
}
