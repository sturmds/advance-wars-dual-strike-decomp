#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov006_0235f690.h"

void Palette_QueueBlockCopy(int id, int army, void* dest, int copy);
const void* Terrain_GetIconTiles(int id);
const void* Terrain_GetPalette(int id, int army);
void TransferQueue_Add(const void* src, void* dst, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern Unk_027c0288 data_027c0288;
extern Unk_027c0280* data_027c0280;
extern Unk_02080440* data_027c027c;
extern s8 data_ov006_0236a7a0[];

/* Loads the tiles (two blocks in the sub screen object VRAM, at 0x5280 and
 * 0x5380) and the palette (offset 0x260) for the id in table data_ov006_0236a7a0
 * at index unk_20, using tileArmy as the army. The sub screen state is current
 * while the palette loads. */
void Ov006_LoadEntryTerrainGraphics(void) {
  int id = data_ov006_0236a7a0[data_ov006_0236d640->unk_20];
  int army = data_ov006_0236d640->tileArmy;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  Palette_QueueBlockCopy(
      id, army, ((Unk_027c0280*)data_027c0288.unk_04)->objVram + 0x5280, 1);
  TransferQueue_Add(Terrain_GetIconTiles(id),
                    ((Unk_027c0280*)data_027c0288.unk_04)->objVram + 0x5380,
                    0x100);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Palette_Load(Terrain_GetPalette(id, army), 0x260, 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
