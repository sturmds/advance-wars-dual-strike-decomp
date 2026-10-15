#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "sdk/os.h"
#include "sdk/gx_load.h"

int func_0210c610(void); /* same three for the sub screen */

/* Copies size bytes to the background extended palette of the current screen
 * (main when it is the first screen, else sub): slot (8 KB each) plus offset. */
void BgExtPalette_Load(const void* src, u32 offset, u32 size, u32 slot) {
  if (data_027c027c == data_027c0288.unk_00) {
    DC_FlushRange(src, size);
    GX_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(src, offset + (slot << 13), size);
    GX_EndLoadBGExtPltt();
  } else {
    DC_FlushRange(src, size);
    func_0210c610();
    GXS_LoadBGExtPltt(src, offset + (slot << 13), size);
    GXS_EndLoadBGExtPltt();
  }
}
