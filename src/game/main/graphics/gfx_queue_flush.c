#include "types.h"
#include "game/gfx_queue.h"
#include "sdk/gx_load.h"

void Gfx_LoadTexImage(const void* src, u32 offset, u32 size);
void Gfx_LoadTexPalette(const void* src, u32 offset, u32 size);
void GfxQueue_ClearAfterFlush(void);

/* Texture palette VRAM while bank F is mapped to the LCD controller. */
#define VRAM_LCDC_F ((volatile u16*)0x06890000)

/* Runs the queued graphics copies during VBlank. Kinds 1 and 3 copy texture
 * data, kinds 2 and 4 texture palettes; for kinds 2 and 3, a 2-byte entry
 * writes one palette colour straight into VRAM. Then clears the queue. */
void GfxQueue_Flush(void) {
  int i;

  if (data_027c051c == 0) {
    return;
  }
  for (i = 0; i < data_027c051c; i++) {
    switch (data_027c0520[i].kind) {
      case 1:
        Gfx_LoadTexImage(data_027c0520[i].src, (u32)data_027c0520[i].dst,
                         data_027c0520[i].size);
        break;
      case 2:
        if (data_027c0520[i].size == 2) {
          GX_BeginLoadTexPltt();
          VRAM_LCDC_F[(u32)data_027c0520[i].dst / 2] =
              *(const u16*)data_027c0520[i].src;
          GX_EndLoadTexPltt();
        } else {
          Gfx_LoadTexPalette(data_027c0520[i].src, (u32)data_027c0520[i].dst,
                             data_027c0520[i].size);
        }
        break;
      case 3:
        if (data_027c0520[i].size == 2) {
          GX_BeginLoadTexPltt();
          VRAM_LCDC_F[(u32)data_027c0520[i].dst / 2] =
              *(const u16*)data_027c0520[i].src;
          GX_EndLoadTexPltt();
        } else {
          Gfx_LoadTexImage(data_027c0520[i].src, (u32)data_027c0520[i].dst,
                           data_027c0520[i].size);
        }
        break;
      case 4:
        Gfx_LoadTexPalette(data_027c0520[i].src, (u32)data_027c0520[i].dst,
                           data_027c0520[i].size);
        break;
    }
  }
  GfxQueue_ClearAfterFlush();
}
