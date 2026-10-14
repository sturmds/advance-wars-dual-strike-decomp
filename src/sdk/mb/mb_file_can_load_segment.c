#include "types.h"
#include "sdk/mb.h"

const u32 gMbSegmentTypes[DLPLAY_PROGRAM_PART_COUNT] = {
#include "data/main/gMbSegmentTypes.inc.c"
};
const u32 data_02151978[6] = {
#include "data/main/data_02151978.inc.c"
};

BOOL MbFile_CanReceiveSegment(u8 segment, u32 address, u32 size);

/* Whether a program segment of this size can be loaded at this address. The
 * ARM7 static may also go past its receive buffer, or into WRAM. */
BOOL MbFile_CanLoadSegment(u8 segment, u32 address, u32 size) {
  switch (gMbSegmentTypes[segment]) {
    case DLPLAY_PART_MAINCPU_CODE:
    case DLPLAY_PART_CART_HEADER:
      return MbFile_CanReceiveSegment(segment, address, size);

    case DLPLAY_PART_SUBCPU_CODE:
      if (address >= DLPLAY_MAIN_RAM_BASE && address < DLPLAY_HOST_DESC_SLOT) {
        u32 end = address + size;

        if (address < DLPLAY_SUBCPU_CODE_AREA_END &&
            end > DLPLAY_SUBCPU_CODE_AREA_END) {
          return FALSE;
        }
        if (end <= DLPLAY_SUBCPU_CODE_AREA_END) {
          return TRUE;
        }
        if (end < DLPLAY_HOST_DESC_SLOT &&
            size <= DLPLAY_SUBCPU_CODE_SIZE_LIMIT) {
          return TRUE;
        }
        return FALSE;
      }
      if (address >= DLPLAY_FAST_RAM_START && address < DLPLAY_FAST_RAM_END) {
        return address + size <= DLPLAY_FAST_RAM_END;
      }
      break;

    default:
      return FALSE;
  }
  return FALSE;
}
