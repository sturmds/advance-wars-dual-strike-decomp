#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/menu_state.h"
#include "game/ov003_unit.h"
#include "sdk/gx_load.h"

void Data_Unpack(const void* src, void* dst);
void Mem_Copy32(const void* src, void* dst, u32 size);

/* VRAM banks A and F while they are mapped to the CPU. */
#define VRAM_LCDC_A(offset) ((void*)(0x06800000 + (offset)))
#define VRAM_LCDC_F(offset) ((void*)(0x06890000 + (offset)))

/* The palettes the row colours pick from, one per colour. */
#define ROW_PALETTES_SIZE (OV003_ROW_COLOR_COUNT * PALETTE_16_SIZE)

/* Unpacks graphics from file (and one image from the localized file) into VRAM
 * bank A and copies palettes into bank F. The 160-byte palette block is also kept
 * in gMenuState; each row in use gets the 32-byte palette its color (1-based)
 * picks from it, at palette slot 11 + row. */
void Ov003_LoadMainScreenGraphics(u8* file, void* localizedFile) {
  int i;

  GX_BeginLoadTex();
  Data_Unpack(file, VRAM_LCDC_A(0xd80));
  Data_Unpack(file + 0x1124, VRAM_LCDC_A(0x6780));
  Data_Unpack(file + 0x1194, VRAM_LCDC_A(0x68c0));
  Data_Unpack(file + 0x11b8, VRAM_LCDC_A(0x6900));
  Data_Unpack(file + 0x1b58, VRAM_LCDC_A(0x8900));
  Data_Unpack(localizedFile, VRAM_LCDC_A(0x8f80));
  Data_Unpack(file + 0x2310, VRAM_LCDC_A(0x9a20));
  Data_Unpack(file + 0x24f0, VRAM_LCDC_A(0x9ca0));
  Data_Unpack(file + 0x1d30, VRAM_LCDC_A(0xa4a0));
  GX_EndLoadTex();
  GX_BeginLoadTexPltt();
  Mem_Copy32(file + 0x1084, VRAM_LCDC_F(0x160), ROW_PALETTES_SIZE);
  Mem_Copy32(file + 0x1084, &gMenuState, ROW_PALETTES_SIZE);
  for (i = 0; i < data_ov003_02364928->rowCount; i++) {
    Mem_Copy32((u8*)&gMenuState +
                   (data_ov003_02364928->rows[i].color - 1) * PALETTE_16_SIZE,
               VRAM_LCDC_F((i + 11) * PALETTE_16_SIZE), PALETTE_16_SIZE);
  }
  Mem_Copy32(file + 0x1cf0, VRAM_LCDC_F(0x200), PALETTE_16_SIZE);
  Mem_Copy32(file + 0x1d10, VRAM_LCDC_F(0x220), PALETTE_16_SIZE);
  Mem_Copy32(file + 0x24d0, VRAM_LCDC_F(0x240), PALETTE_16_SIZE);
  Mem_Copy32(file + 0x22f0, VRAM_LCDC_F(0x260), PALETTE_16_SIZE);
  Mem_Copy32(file + 0x2684, VRAM_LCDC_F(0x280), PALETTE_16_SIZE);
  GX_EndLoadTexPltt();
}
