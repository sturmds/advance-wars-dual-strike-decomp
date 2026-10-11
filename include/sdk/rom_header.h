#ifndef SDK_ROM_HEADER_H
#define SDK_ROM_HEADER_H

#include "types.h"

/* Where a table lies in the cartridge ROM. */
typedef struct RomRegion {
    u32 offset;
    u32 length;
} RomRegion;

/* The cartridge header fields the restart code and the FS library read. */
typedef struct RomHeader {
    u8 pad_00[0x20];
    u32 arm9RomOffset;
    u32 pad_24;
    u32 arm9RamAddress;
    u32 arm9Size;
    u32 arm7RomOffset;
    u32 pad_34;
    u32 arm7RamAddress;
    u32 arm7Size;
    RomRegion fnt;                 /* file name table */
    RomRegion fat;                 /* file allocation table */
    RomRegion arm9Overlays;        /* ARM9 overlay table */
    RomRegion arm7Overlays;        /* ARM7 overlay table */
    u32 cardControl;               /* ROMCTRL setting for normal card reads */
    u8 pad_64[0xfc];
} RomHeader;

/* Copy of the cartridge header in main RAM, 0x160 bytes. */
#define ROM_HEADER_BUF ((RomHeader *)0x027ffe00)

#endif
