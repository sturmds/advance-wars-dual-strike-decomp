#include "types.h"

void func_0210c8e8(void);
void func_0210c874(const void *src, u32 offset, u32 size);  /* guess: copies to texture palette VRAM */
void GX_EndLoadTexPltt(void);

/* Copies size bytes of texture palette data to offset in texture palette
 * VRAM. func_0210c8e8 and GX_EndLoadTexPltt wrap the copy (guess: they open and
 * close VRAM access for the write). */
void Gfx_LoadTexPalette(const void *src, u32 offset, u32 size)
{
    func_0210c8e8();
    func_0210c874(src, offset, size);
    GX_EndLoadTexPltt();
}
