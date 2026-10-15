#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"

void Palette_QueueBlockCopy(int id, int army, void* dest, int copy);

/* Loads the tiles of each terrain entry of data_ov006_0236a7a0 for the
 * given army into the object VRAM, 8 tiles apart from tile 0x25e. */
void Ov006_LoadTerrainTiles(int army) {
  int i;
  int tile = 0x25e;

  for (i = 0; i < (int)ARRAY_COUNT(data_ov006_0236a7a0.ids); i++) {
    Palette_QueueBlockCopy(data_ov006_0236a7a0.ids[i], army,
                           data_027c0280->objVram + tile * 32, 1);
    tile += 8;
  }
}
