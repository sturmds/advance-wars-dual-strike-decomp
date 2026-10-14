#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"

void Palette_QueueBlockCopy(int id, int army, void* dest, int copy);
const void* Terrain_GetPalette(int id, int army);
void Palette_Load(const void* src, u32 offset, u32 size);

extern Unk_027c0280* data_027c0280;

/* Loads the tiles for an army into object VRAM, 8 tiles apart: the property
 * entries of data_ov006_0236a7a0, or all but the HQ (from tile 0x2c6) when
 * army is 0.
 * Then loads palette 6 of the army at offset 0x300. */
void Ov006_LoadArmyTiles(int army) {
  s8* ids;
  int tile;
  int i;
  int count;

  if (army != 0) {
    ids = &data_ov006_0236a7a0.ids[OV006_PROPERTY_ENTRY_FIRST];
    tile = 0x2be;
    count = OV006_PROPERTY_ENTRY_COUNT;
  } else {
    /* all but the HQ */
    ids = &data_ov006_0236a7a0.ids[OV006_PROPERTY_ENTRY_FIRST + 1];
    tile = 0x2c6;
    count = OV006_PROPERTY_ENTRY_COUNT - 1;
  }
  for (i = 0; i < count; i++) {
    Palette_QueueBlockCopy(*ids, army, data_027c0280->objVram + tile * 32, 1);
    tile += 8;
    ids++;
  }
  Palette_Load(Terrain_GetPalette(6, army), 0x300, 32);
}
