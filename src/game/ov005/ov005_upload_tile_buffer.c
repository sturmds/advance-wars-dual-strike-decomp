#include "types.h"
#include "game/Unk_ov005_0235b574.h"

void func_ov005_0235b5dc(Unk_ov005_0235b574 *work, int x, int y);
void DC_FlushRange(const void *start, u32 size);  /* guess: flushes the data cache for the range */
void func_0210cb04(void);                          /* guess: begins a VRAM load */
void func_0210c9a0(const void *src, u32 offset, u32 size);
void GX_EndLoadTex(void);                          /* guess: ends the load */

void Ov005_UploadTileBuffer(Unk_ov005_0235b574 *work)
{
    int i;

    for (i = 0; i < 128; i++) {
        func_ov005_0235b5dc(work, i, i);
    }
    DC_FlushRange(work->buffer_6010, 0x2000);
    func_0210cb04();
    func_0210c9a0(work->buffer_6010, 0, 0x2000);
    GX_EndLoadTex();
}
