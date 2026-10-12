#include "sdk/fs.h"
#include "sdk/os.h"

/* Digests of the overlays a downloaded program may run. This ROM has none:
 * both ends are at the same address. */
extern OverlayHmacBytes gOverlayExpectedHmacs[];
extern OverlayHmacBytes gOverlayExpectedHmacsEnd[];

u32 Overlay_FileReadSize(const FSOverlayInfo *info);
BOOL Overlay_CheckKeyedHash(const u8 *expected, const void *data, u32 size);
void MI_CpuFill8(void *dest, u8 value, u32 size);
void MIi_UncompressBackward(void *bottom);
/* Debugger support library: records the overlay's range for the debugger. */
int Isd_RegisterOverlay(MIProcessor target, void *address, u32 fileOffset, u32 size);

typedef void (*FSOverlayInitFunc)(void);

/* Runs an overlay that has been read into memory. A program started by
 * Download Play first checks the overlay's digest and stops for good when it
 * does not match. Then the overlay is decompressed if needed, flushed from
 * the data cache, and its static initializers run. */
void FS_StartOverlay(FSOverlayInfo *info)
{
    u32 binarySize = Overlay_FileReadSize(info);

    if (*BOOT_ORIGIN_WORD == BOOT_ORIGIN_WIRELESS) {
        BOOL valid = FALSE;
        if (info->header.flag & OVERLAY_NEEDS_HMAC_CHECK) {
            u32 id = info->header.id;
            if (id < (u32)(gOverlayExpectedHmacsEnd - gOverlayExpectedHmacs)) {
                valid = Overlay_CheckKeyedHash(gOverlayExpectedHmacs[id].bytes, info->header.ram_address, binarySize);
            }
        }
        if (!valid) {
            MI_CpuFill8(info->header.ram_address, 0, binarySize);
            OS_Terminate();
            return;
        }
    }
    if (info->header.flag & OVERLAY_STORED_PACKED) {
        MIi_UncompressBackward(info->header.ram_address + binarySize);
    }
    DC_FlushRange(info->header.ram_address, info->header.ram_size);
    {
        u32 size = info->header.ram_size;  /* needed to match: loaded before the other arguments */
        Isd_RegisterOverlay(info->target == MI_PROCESSOR_ARM9 ? MI_PROCESSOR_ARM9 : MI_PROCESSOR_ARM7,
                      info->header.ram_address, info->romTop, size);
    }
    {
        FSOverlayInitFunc *init = info->header.sinit_init;
        FSOverlayInitFunc *end = info->header.sinit_init_end;
        for (; init < end; init++) {
            if (*init != NULL) {
                (*init)();
            }
        }
    }
}
