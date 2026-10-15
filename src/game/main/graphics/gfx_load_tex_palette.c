#include "types.h"
#include "sdk/gx_load.h"

/* Copies size bytes of texture palette data to offset in texture palette
 * VRAM. GX_BeginLoadTexPltt and GX_EndLoadTexPltt wrap the copy (guess: they open and
 * close VRAM access for the write). */
void Gfx_LoadTexPalette(const void* src, u32 offset, u32 size) {
  GX_BeginLoadTexPltt();
  GX_LoadTexPltt(src, offset, size);
  GX_EndLoadTexPltt();
}
