#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Ov003Fx.h"
#include "game/screen_other_map.h"

void Palette_Load(const void* src, int offset, u32 size);
#define EFFECT_TILE_VRAM ((u8*)0x0661e000) /* sub-screen OBJ VRAM */
/* Bytes of one tile image copied: two 4bpp tiles. */
#define EFFECT_IMAGE_SIZE (2 * TILE_4BPP_SIZE)

#define BG0_CHAR_BASE      \
  (data_027c0280->bgVram + \
   (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14))

void Mem_Copy32(const void* src, void* dst, u32 size);

/* Frame callback: loads the two colours into palette offsets 0x13a and 0x15a
 * (the same colours as Ov003_WriteFxColors), then, if unk_0b has a request,
 * copies two 0x40-byte tile images into BG 0's tile data. */
void Ov003_UploadFxColorsAndTiles(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (data_ov003_02364a84->mode == 0) {
    Palette_Load(&data_ov003_02364a84->colors, 0x13a, 4);
    Palette_Load(&data_ov003_02364a84->colors, 0x15a, 4);
  } else if (data_ov003_02364a84->mode == 11) {
    Palette_Load(data_ov003_02364a84, 0x13a, 4);
    Palette_Load(data_ov003_02364a84, 0x15a, 4);
  }
  if (data_ov003_02364a84->unk_0b_0 != 0) {
    Mem_Copy32(EFFECT_TILE_VRAM + data_ov003_02364a84->unk_0b_2 * 0x80,
               BG0_CHAR_BASE + 0x57c0, EFFECT_IMAGE_SIZE);
    Mem_Copy32(
        EFFECT_TILE_VRAM + (data_ov003_02364a84->unk_0b_2 * 4 + 2) * 0x20,
        BG0_CHAR_BASE + 0x5bc0, EFFECT_IMAGE_SIZE);
    data_ov003_02364a84->unk_0b_0 = 0;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
