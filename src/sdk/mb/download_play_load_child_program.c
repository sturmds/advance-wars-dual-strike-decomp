#include "types.h"
#include "sdk/fs.h"
#include "sdk/mb.h"
#include "sdk/rom_header.h"
#include "sdk/os.h"

extern const RomRegion *gMbArm9CopyRegions;
extern const char gMbRomArchiveName[];

/* Needed to match: found by tools/pragmas.py; the original file was built
 * with these optimisations off. */
#pragma opt_dead_assignments off
#pragma opt_common_subs off
#pragma opt_loop_invariants off

void AutoloadCallback(int arg);
void Mem_CopyBytes(const void *src, void *dest, u32 size);
void func_0211daf4(DlPlayHeldProgram *cache);        /* clear the cache */
void MbCache_Add(DlPlayHeldProgram *cache, u32 address, u32 size, u8 *buffer, u32 state);
void MbSegment_CopySecureArea(DlPlayLiveCodePatch *copy, u32 start, u32 end, BOOL clear);

/* Fills buffer with what DownloadPlay_OfferChildProgram needs to offer a child program:
 * ROM header, auth code, cache, card map, then the ARM9 and ARM7 static
 * code (or the first 0xcc00 bytes of ARM9 code when both do not fit).
 * file NULL means the running program itself: its header comes from RAM,
 * its secure area is cleared except the parts copied from RAM, and its
 * AutoloadCallback becomes a plain return. */
BOOL DownloadPlay_LoadChildProgram(FSFile *file, void *buffer, u32 length)
{
    BOOL result = FALSE;

    if (length >= sizeof(RomHeader) + 4) {
        RomHeader *header = buffer;
        BOOL ownProgram = FALSE;           /* file was NULL */
        u32 rest = length;
        u8 *next = buffer;
        DlPlayHeldProgram *cache = NULL;
        DlPlayProgramRomOffsets *mapping = NULL;
        u32 base;
        u32 romSize;
        FSFile ownFile;

        next += sizeof(RomHeader);
        rest -= sizeof(RomHeader);
        if (file != NULL) {
            base = file->pos - file->top;
            if (FS_ReadFile(file, buffer, sizeof(RomHeader)) < (s32)sizeof(RomHeader)) {
                rest = 0;
            }
            romSize = header->romSize;
            if (romSize == 0) {
                romSize = DLPLAY_ASSUMED_CART_BYTES;
            }
        } else {
            const RomHeader *ownHeader = ROM_HEADER_BUF;

            romSize = ownHeader->romSize;
            if (romSize == 0) {
                romSize = DLPLAY_ASSUMED_CART_BYTES;
            }
            ownProgram = TRUE;
            FS_InitFile(&ownFile);
            FS_OpenFileDirect(&ownFile, FS_FindArchive(gMbRomArchiveName, 3), 0, romSize + DLPLAY_SIGNATURE_BYTES, (u32)-1);
            file = &ownFile;
            base = file->pos - file->top;
            Mem_CopyBytes(ownHeader, buffer, sizeof(RomHeader));
            header->cardControl |= 0x406000;   /* card read settings for the child */
        }

        if (rest >= DLPLAY_SIGNATURE_BYTES) {
            FS_SeekFile(file, base + romSize, FS_SEEK_SET);
            FS_ReadFile(file, next, DLPLAY_SIGNATURE_BYTES);
            next += DLPLAY_SIGNATURE_BYTES;
            rest -= DLPLAY_SIGNATURE_BYTES;
        } else {
            rest = 0;
        }

        if (rest >= sizeof(DlPlayHeldProgram)) {
            const s8 *name;
            int nameLength;

            cache = (DlPlayHeldProgram *)next;
            func_0211daf4(cache);
            next += sizeof(DlPlayHeldProgram);
            rest -= sizeof(DlPlayHeldProgram);
            MbCache_Add(cache, 0, sizeof(RomHeader), buffer, DLPLAY_HELD_BLOCK_PINNED);
            name = (const s8 *)&file->arc->name;
            for (nameLength = 0; nameLength < 3 && name[nameLength] != 0; nameLength++) {
            }
            Mem_CopyBytes(name, cache->archiveName, nameLength);
            cache->archiveNameLength = nameLength;
        } else {
            rest = 0;
        }

        if (rest >= sizeof(DlPlayProgramRomOffsets)) {
            mapping = (DlPlayProgramRomOffsets *)next;
            mapping->headerRomAddress = 0;
            mapping->arm9RomAddress = header->arm9RomOffset + (base + file->top);
            mapping->arm7RomAddress = header->arm7RomOffset + (base + file->top);
            next += sizeof(DlPlayProgramRomOffsets);
            rest -= sizeof(DlPlayProgramRomOffsets);
        } else {
            rest = 0;
        }

        if (rest >= header->arm9Size + header->arm7Size) {
            u32 top = file->top;

            FS_SeekFile(file, mapping->arm9RomAddress - top, FS_SEEK_SET);
            FS_ReadFile(file, next, header->arm9Size);
            MbCache_Add(cache, mapping->arm9RomAddress, header->arm9Size, next, DLPLAY_HELD_BLOCK_PINNED);
            next += header->arm9Size;
            FS_SeekFile(file, mapping->arm7RomAddress - top, FS_SEEK_SET);
            FS_ReadFile(file, next, header->arm7Size);
            MbCache_Add(cache, mapping->arm7RomAddress, header->arm7Size, next, DLPLAY_HELD_BLOCK_PINNED);
            result = TRUE;
        } else if (rest >= 3 * DLPLAY_MAINCPU_SPLIT_BYTES) {
            u32 top = file->top;
            u32 address = mapping->arm9RomAddress;

            FS_SeekFile(file, address - top, FS_SEEK_SET);
            FS_ReadFile(file, next, DLPLAY_MAINCPU_SPLIT_BYTES);
            MbCache_Add(cache, address, DLPLAY_MAINCPU_SPLIT_BYTES, next, DLPLAY_HELD_BLOCK_PINNED);
            FS_SeekFile(file, address + DLPLAY_MAINCPU_SPLIT_BYTES - top, FS_SEEK_SET);
            FS_ReadFile(file, next + DLPLAY_MAINCPU_SPLIT_BYTES, DLPLAY_MAINCPU_SPLIT_BYTES);
            MbCache_Add(cache, address + DLPLAY_MAINCPU_SPLIT_BYTES, DLPLAY_MAINCPU_SPLIT_BYTES, next + DLPLAY_MAINCPU_SPLIT_BYTES, DLPLAY_HELD_BLOCK_USABLE);
            FS_SeekFile(file, address + 2 * DLPLAY_MAINCPU_SPLIT_BYTES - top, FS_SEEK_SET);
            FS_ReadFile(file, next + 2 * DLPLAY_MAINCPU_SPLIT_BYTES, DLPLAY_MAINCPU_SPLIT_BYTES);
            MbCache_Add(cache, address + 2 * DLPLAY_MAINCPU_SPLIT_BYTES, DLPLAY_MAINCPU_SPLIT_BYTES, next + 2 * DLPLAY_MAINCPU_SPLIT_BYTES, DLPLAY_HELD_BLOCK_USABLE);
            result = TRUE;
        }

        FS_SeekFile(file, base, FS_SEEK_SET);
        if (ownProgram) {
            FS_CloseFile(&ownFile);
            if (result) {
                DlPlayLiveCodePatch copy;
                const RomRegion *region;
                u32 *patch;

                copy.romOffset = header->arm9RomOffset;
                region = gMbArm9CopyRegions;
                copy.source = (const u8 *)(header->arm9RamAddress - header->arm9RomOffset);
                copy.dest = cache->pages[1].buffer - header->arm9RomOffset;
                copy.length = length;
                MbSegment_CopySecureArea(&copy, DLPLAY_PROTECTED_ROM_START, DLPLAY_PROTECTED_ROM_END, TRUE);
                for (; region->length != 0; region++) {
                    MbSegment_CopySecureArea(&copy, region->offset, region->offset + region->length, FALSE);
                }
                patch = (u32 *)(cache->pages[1].buffer + ((u32)AutoloadCallback - header->arm9RamAddress));
                *patch = DLPLAY_RETURN_OPCODE;
            }
        }
        if (result) {
            DC_FlushRange(buffer, length);
        }
    }
    return result;
}
