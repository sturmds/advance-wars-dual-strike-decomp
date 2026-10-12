#include "types.h"
#include "sdk/mb.h"

void MI_CpuFill8(void* dest, int value, u32 size);
void Mem_CopyBytes(const void* src, void* dest, u32 size);

/* Copies ROM offsets [start, end) of the ARM9 static code, or clears them,
 * limited to the secure area and to the range the copy covers. */
void MbSegment_CopySecureArea(DlPlayLiveCodePatch* copy, u32 start, u32 end,
                              BOOL clear) {
  u32 romOffset = copy->romOffset;
  u32 length = copy->length;

  if (start < DLPLAY_PROTECTED_ROM_START) {
    start = DLPLAY_PROTECTED_ROM_START;
  }
  if (end > DLPLAY_PROTECTED_ROM_END) {
    end = DLPLAY_PROTECTED_ROM_END;
  }
  if (start < romOffset) {
    start = romOffset;
  }
  if (end > romOffset + length) {
    end = romOffset + length;
  }
  if (start >= end) {
    return;
  }
  if (clear) {
    MI_CpuFill8(copy->dest + start, 0, end - start);
  } else {
    Mem_CopyBytes(copy->source + start, copy->dest + start, end - start);
  }
}
