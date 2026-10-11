#include "types.h"

void *Ov004_GetResourceData(const char *name);
void DC_FlushRange(const void *start, u32 size);  /* guess: flushes the data cache for the range */
void func_0210c718(void);                          /* guess: begins a background extended palette load */
void GX_LoadBGExtPltt(const void *src, u32 offset, u32 size);
void GX_EndLoadBGExtPltt(void);                          /* guess: ends the load */

/* Loads a 0x200-byte palette from a resource into the extended palette at
 * 0x6000 (guess: background 3's slot) plus paletteOffset. */
void Ov004_LoadResourcePalette(const char *name, int dataOffset, int paletteOffset) {
    u8 *palette = (u8 *)Ov004_GetResourceData(name) + dataOffset;

    DC_FlushRange(palette, 0x200);
    func_0210c718();
    GX_LoadBGExtPltt(palette, paletteOffset + 0x6000, 0x200);
    GX_EndLoadBGExtPltt();
}
