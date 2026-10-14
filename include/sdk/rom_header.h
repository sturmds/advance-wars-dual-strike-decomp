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
  u32 arm9EntryAddress;
  u32 arm9RamAddress;
  u32 arm9Size;
  u32 arm7RomOffset;
  u32 arm7EntryAddress;
  u32 arm7RamAddress;
  u32 arm7Size;
  RomRegion fnt;          /* file name table */
  RomRegion fat;          /* file allocation table */
  RomRegion arm9Overlays; /* ARM9 overlay table */
  RomRegion arm7Overlays; /* ARM7 overlay table */
  u32 cardControl;        /* ROMCTRL setting for normal card reads */
  u8 pad_64[0x1c];
  u32 romSize; /* 0x80: used size of the cartridge image */
  u8 pad_84[0xdc];
} RomHeader;

typedef char RomHeader_size_check[sizeof(RomHeader) == 0x160 ? 1 : -1];

/* Copy of the cartridge header in main RAM, and where a Download Play
   child keeps it. */
#define ROM_HEADER_BUF ((RomHeader*)0x027ffe00)
#define ROM_HEADER_BUF_DOWNLOAD_PLAY ((RomHeader*)0x023fe940)

#endif
