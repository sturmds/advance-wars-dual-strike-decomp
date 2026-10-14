#include "types.h"
#include "game/Ov003Fx.h"
#include "game/screen_other_map.h"

/* Tile frames the effect copies from (0x06600000 is sub-screen OBJ VRAM). */
#define EFFECT_TILE_VRAM ((u8*)0x06610000)
#define EFFECT_TILE_VRAM_SECOND ((u8*)0x06618000)
/* Bytes per copy: unk_09 sends a frame in two halves, unk_0a one block. */
#define EFFECT_HALF_FRAME_SIZE 0x1000
#define EFFECT_BLOCK_SIZE 0xc00

#define BG0_CHAR_BASE      \
  (data_027c0280->bgVram + \
   (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14))

void Mem_Copy32(const void* src, void* dst, u32 size);

/* Frame callback: copies one chunk of effect tiles into BG 0's tile data,
 * for the first request that is pending: unk_09 (2 steps of 0x1000 bytes),
 * then unk_0a (0xc00 bytes). A request that copied returns at once, without
 * putting the screens back (needed to match; the original does the same). */
void Ov003_UploadFxTiles(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (data_ov003_02364a84->unk_09_0 == 1) {
    Mem_Copy32(EFFECT_TILE_VRAM + data_ov003_02364a84->unk_09_2 * 0x2000,
               BG0_CHAR_BASE + 0x2000, EFFECT_HALF_FRAME_SIZE);
    data_ov003_02364a84->unk_09_0++;
    return;
  } else if (data_ov003_02364a84->unk_09_0 == 2) {
    Mem_Copy32(
        EFFECT_TILE_VRAM + (data_ov003_02364a84->unk_09_2 * 2 + 1) * 0x1000,
        BG0_CHAR_BASE + 0x3000, EFFECT_HALF_FRAME_SIZE);
    data_ov003_02364a84->unk_09_0 = 0;
    return;
  } else if (data_ov003_02364a84->unk_0a_0 != 0) {
    Mem_Copy32(
        EFFECT_TILE_VRAM_SECOND + (data_ov003_02364a84->unk_0a_2 * 0x60 << 5),
        BG0_CHAR_BASE + 0x4000, EFFECT_BLOCK_SIZE);
    data_ov003_02364a84->unk_0a_0 = 0;
    return;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
