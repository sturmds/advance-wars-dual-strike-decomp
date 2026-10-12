#include "types.h"
#include "sdk/mb.h"

/* Whether a program segment of this size can be received at this address. */
BOOL MbFile_CanReceiveSegment(u8 segment, u32 address, u32 size) {
  switch (gMbSegmentTypes[segment]) {
    case DLPLAY_PART_CART_HEADER:
      if (address >= DLPLAY_HEADER_COPY_START &&
          address + size <= DLPLAY_HEADER_COPY_END) {
        return TRUE;
      }
      break;

    case DLPLAY_PART_MAINCPU_CODE:
      if (address >= DLPLAY_MAIN_RAM_BASE &&
          address + size <= DLPLAY_MAINCPU_CODE_AREA_END) {
        return TRUE;
      }
      break;

    case DLPLAY_PART_SUBCPU_CODE:
      /* The second test covers the first; the original has both. */
      if (address >= DLPLAY_MAINCPU_CODE_AREA_END &&
          address + size <= DLPLAY_SUBCPU_CODE_AREA_END) {
        return TRUE;
      }
      if (address >= DLPLAY_MAIN_RAM_BASE &&
          address + size <= DLPLAY_SUBCPU_CODE_AREA_END) {
        return TRUE;
      }
      break;

    default:
      return FALSE;
  }
  return FALSE;
}
