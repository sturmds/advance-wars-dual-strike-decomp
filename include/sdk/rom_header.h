#ifndef SDK_ROM_HEADER_H
#define SDK_ROM_HEADER_H

#include "types.h"

/* The cartridge header fields the restart code reads. */
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
    u8 pad_40[0x20];
    u32 cardControl;               /* ROMCTRL setting for normal card reads */
    u8 pad_64[0xfc];
} RomHeader;

/* Copy of the cartridge header in main RAM, 0x160 bytes. */
#define ROM_HEADER_BUF ((RomHeader *)0x027ffe00)

#endif
