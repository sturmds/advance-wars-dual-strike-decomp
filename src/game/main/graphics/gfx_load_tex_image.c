#include "types.h"

void GX_BeginLoadTex(void);
void func_0210c9a0(const void *src, u32 offset, u32 size);
void GX_EndLoadTex(void);

/* Copies size bytes of texture image to offset in texture VRAM, wrapping the
 * copy in the begin and end calls of the SDK texture loader. */
void Gfx_LoadTexImage(const void *src, u32 offset, u32 size)
{
    GX_BeginLoadTex();
    func_0210c9a0(src, offset, size);
    GX_EndLoadTex();
}
