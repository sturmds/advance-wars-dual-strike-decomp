#include "types.h"
#include "sdk/fs.h"
#include "sdk/mb.h"
#include "sdk/os.h"
#include "sdk/rom_header.h"

/* Fills one segment's info from the child program's ROM header. A static
 * module outside its load area stops the system. The ARM7 static is
 * received at its load address when it fits below the receive limit;
 * otherwise it is received at *relocBuffer, which moves past it, and the
 * child copies it into place. */
void MbFile_SetSegmentInfo(const RomHeader *romHeader, const u32 *segmentType, DlPlaySegmentPlacement *segment, u32 *relocBuffer)
{
    switch (*segmentType) {
    case DLPLAY_PART_MAINCPU_CODE: {
        /* needed to match: address and size read as one RomRegion */
        const RomRegion *region = (const RomRegion *)&romHeader->arm9RamAddress;

        if (region->offset >= DLPLAY_MAIN_RAM_BASE && region->offset < DLPLAY_MAINCPU_CODE_AREA_END
            && region->offset + region->length <= DLPLAY_MAINCPU_CODE_AREA_END) {
            segment->size = region->length;
            segment->loadAddress = region->offset;
            segment->recvAddress = segment->loadAddress;
            segment->target = MI_PROCESSOR_ARM9;
        } else {
            OS_Terminate();
        }
        break;
    }

    case DLPLAY_PART_SUBCPU_CODE: {
        BOOL error = FALSE;
        BOOL relocate = FALSE;
        const RomRegion *region = (const RomRegion *)&romHeader->arm7RamAddress;
        u32 end = region->offset + region->length;

        if (region->offset >= DLPLAY_MAIN_RAM_BASE && region->offset < DLPLAY_HOST_DESC_SLOT) {
            if (end <= DLPLAY_SUBCPU_CODE_AREA_END) {
                /* received in place */
            } else if (end < DLPLAY_HOST_DESC_SLOT && region->length <= DLPLAY_SUBCPU_CODE_SIZE_LIMIT) {
                relocate = TRUE;
            } else {
                error = TRUE;
            }
        } else if (region->offset >= DLPLAY_FAST_RAM_START && region->offset < DLPLAY_FAST_RAM_END) {
            if (end <= DLPLAY_FAST_RAM_END) {
                relocate = TRUE;
            } else {
                error = TRUE;
            }
        } else {
            error = TRUE;
        }
        if (error == TRUE) {
            OS_Terminate();
        }
        segment->size = region->length;
        segment->loadAddress = region->offset;
        if (!relocate) {
            segment->recvAddress = segment->loadAddress;
        } else {
            segment->recvAddress = *relocBuffer;
            *relocBuffer += segment->size;
        }
        segment->target = MI_PROCESSOR_ARM7;
        break;
    }

    case DLPLAY_PART_CART_HEADER:
        segment->size = DLPLAY_CART_HEADER_BYTES;
        segment->loadAddress = DLPLAY_HEADER_COPY_START;
        segment->recvAddress = segment->loadAddress;
        segment->target = MI_PROCESSOR_ARM9;
        break;
    }
}
