#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void DC_FlushRange(const void *start, u32 size);  /* guess: flushes the data cache for the range */
void func_0210c718(void);                          /* guess: begins a background extended palette load */
void GX_LoadBGExtPltt(const void *src, u32 offset, u32 size);
void GX_EndLoadBGExtPltt(void);                          /* guess: ends the load */
int func_0210c610(void);                           /* same three for the sub screen */
void GXS_LoadBGExtPltt(const void *src, u32 offset, u32 size);
void GXS_EndLoadBGExtPltt(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0288 data_027c0288;

/* Copies size bytes to the background extended palette of the current screen
 * (main when it is the first screen, else sub): slot (8 KB each) plus offset. */
void BgExtPalette_Load(const void *src, u32 offset, u32 size, u32 slot)
{
    if (data_027c027c == data_027c0288.unk_00) {
        DC_FlushRange(src, size);
        func_0210c718();
        GX_LoadBGExtPltt(src, offset + (slot << 13), size);
        GX_EndLoadBGExtPltt();
    } else {
        DC_FlushRange(src, size);
        func_0210c610();
        GXS_LoadBGExtPltt(src, offset + (slot << 13), size);
        GXS_EndLoadBGExtPltt();
    }
}
