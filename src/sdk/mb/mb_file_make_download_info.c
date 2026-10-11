#include "types.h"
#include "sdk/mb.h"
#include "sdk/rom_header.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);
void MbFile_SetSegmentInfo(const RomHeader *romHeader, const u32 *segmentType, DlPlaySegmentPlacement *segment, u32 *relocBuffer);

/* Builds a file's download info from the buffer given to DownloadPlay_OfferChildProgram:
 * entry points and segments from its ROM header, then the auth code that
 * follows the header. */
void MbFile_MakeDownloadInfo(DlPlayProgramLayout *info, const void *buffer)
{
    /* needed to match: this declaration order */
    const RomHeader *romHeader;
    DlPlaySegmentPlacement *segment;
    u32 relocBuffer;
    const u32 *segmentType;
    int i;
    const u8 *authCode;

    romHeader = buffer;
    authCode = (const u8 *)buffer + DLPLAY_CART_HEADER_BYTES;
    segmentType = gMbSegmentTypes;
    segment = info->segments;
    relocBuffer = DLPLAY_SUBCPU_CODE_STAGING;

    info->arm9EntryAddress = romHeader->arm9EntryAddress;
    info->arm7EntryAddress = romHeader->arm7EntryAddress;
    for (i = 0; i < DLPLAY_PROGRAM_PART_COUNT; i++) {
        MbFile_SetSegmentInfo(romHeader, segmentType, segment, &relocBuffer);
        segment++;
        segmentType++;
    }
    Mem_CopyBytes(authCode, info->authCode, DLPLAY_SIGNATURE_BYTES);
}
