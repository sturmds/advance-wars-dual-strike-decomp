#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba8.h"

void *Mem_Alloc(u32 size);
void Mem_Free(void *block);
void Mem_Fill32(u32 value, void *dst, u32 size);
void DC_FlushRange(const void *start, u32 size);  /* guess: flushes the data cache for the range */
void func_02028808(void);
void Co_LoadTexture(int coId, int tileOffset, int paletteOffset);
void Gfx_LoadTexPalette(void *palette, int start, int offset);

void func_ov005_02363014(Ov005Task *task)
{
    void *blank;
    int i;
    int loaded;
    Ov005CoGridEntry *entry;
    u8 last;
    int tileOffset;
    int paletteOffset;
    entry = task->work;
    blank = Mem_Alloc(32);

    Mem_Fill32(0, blank, 32);
    DC_FlushRange(blank, 32);
    func_02028808();
    loaded = 0;
    i = 0;
    paletteOffset = 0;
    tileOffset = 0;
    entry = (Ov005CoGridEntry *)((u8 *)entry + 0x3c);
    for (; i < 28; i++, entry++) {
        if (entry->coId == -1) {
            continue;
        }
        Co_LoadTexture(entry->coId, tileOffset, paletteOffset);
        if (entry->available == 0) {
            Gfx_LoadTexPalette(blank, paletteOffset, paletteOffset);
        }
        paletteOffset += 32;
        tileOffset += 0x180;
        loaded++;
        last = loaded;
    }
    Mem_Free(blank);
}
