#include "types.h"
#include "sdk/mb.h"

extern DlPlayHostLinkArea *data_022aced4;

/* Finds the segment that holds a block of a file and fills in where the
 * block is: its offset in the file, its size (at most one message) and
 * where the child receives it. FALSE for a block past the end. */
BOOL MbFile_GetBlockInfo(DlPlayBlockSpot *info, const DlPlaySegmentStarts *table, u16 block, const DlPlayProgramLayout *header)
{
    s8 i;
    u32 segmentBlock;
    u32 offset;
    const DlPlaySegmentPlacement *segment;

    if (block >= table->blockCount) {
        return FALSE;
    }
    for (i = DLPLAY_PROGRAM_PART_COUNT - 1; i >= 0; i--) {
        if (block >= table->segmentFirstBlocks[i]) {
            break;
        }
    }
    if (i < 0) {
        return FALSE;
    }

    segmentBlock = block - table->segmentFirstBlocks[i];
    offset = segmentBlock * data_022aced4->blockSizeMax;
    segment = &header->segments[i];
    info->size = segment->size - offset;
    if (info->size > data_022aced4->blockSizeMax) {
        info->size = data_022aced4->blockSizeMax;
    }
    info->offset = offset + table->segmentOffsets[i];
    info->unk_00 = offset + segment->recvAddress;
    info->segment = i;
    return TRUE;
}
