#include "types.h"
#include "game/Unk_02088d04.h"

void Palette_QueueBlockCopy(int id, int army, void* dest, int copy);

extern Unk_027c0280* data_027c0280;
extern s8 data_ov006_0236a7a0[];

/* Loads the tiles of each of the 18 entries of data_ov006_0236a7a0 for the
 * given army into the object VRAM, 8 tiles apart from tile 0x25e. */
void Ov006_LoadTerrainTiles(int army) {
  int i;
  int tile = 0x25e;

  for (i = 0; i < 18; i++) {
    Palette_QueueBlockCopy(data_ov006_0236a7a0[i], army,
                           data_027c0280->objVram + tile * 32, 1);
    tile += 8;
  }
}
