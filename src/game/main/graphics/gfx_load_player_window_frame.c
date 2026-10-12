#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

u32 Gfx_UnpackBmap09f(void* dst);
void Gfx_LoadPlayerBmap0a0Palette(int player);

/* Unpacks the window frame tiles of /bmap/09f into the VRAM area in
 * unk_f8 and loads the army palette of player. */
void Gfx_LoadPlayerWindowFrame(int player) {
  Gfx_UnpackBmap09f(data_027c0280->unk_f8);
  Gfx_LoadPlayerBmap0a0Palette(player);
}
