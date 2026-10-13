#include "types.h"
#include "sdk/mb.h"

BOOL MbFile_CanLoadSegment(u8 segment, u32 address, u32 size);

/* Fills a file's block table from its download info: where each segment
 * starts in the file and at which block, and the file's block count.
 * FALSE when there is no download info or a segment cannot be loaded where
 * it asks to be. */
BOOL MbFile_MakeBlockTable(DlPlaySegmentStarts* table,
                           const DlPlayProgramLayout* header) {
  u16* firstBlocks = table->segmentFirstBlocks;
  u32 offset = 0;
  u8 i;

  if (header == NULL) {
    return FALSE;
  }
  for (i = 0; i < DLPLAY_PROGRAM_PART_COUNT; i++) {
    table->segmentOffsets[i] = offset;
    offset += header->segments[i].size;
  }

  firstBlocks[0] = 0;
  for (i = 0; i < DLPLAY_PROGRAM_PART_COUNT; i++) {
    const DlPlaySegmentPlacement* segment = &header->segments[i];
    /* needed to match: the block count is narrowed before the add */
    u16 nextBlock = firstBlocks[i] +
                    (u16)((segment->size + data_022aced4->blockSizeMax - 1) /
                          data_022aced4->blockSizeMax);

    if (!MbFile_CanLoadSegment(i, segment->loadAddress, segment->size)) {
      return FALSE;
    }
    if (i < DLPLAY_PROGRAM_PART_COUNT - 1) {
      firstBlocks[i + 1] = nextBlock;
    } else {
      table->blockCount = nextBlock;
    }
  }
  return TRUE;
}
