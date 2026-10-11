#include "types.h"
#include "sdk/rom_header.h"

/* Cartridge offset of the image to load. */
#define ROM_IMAGE_BASE (*(u32 *)0x027ffc2c)
/* ROM bytes below this offset are not copied to the ARM9 destination. */
#define SECURE_AREA_END 0x8000

/* Reads size bytes from the cartridge at romOffset into dest. */
void OSi_ReadCardRom32(u32 romOffset, void *dest, u32 size);
void func_0210e0d4(void);
void func_0210e154(void);
void func_0210e160(void);

/* Loads the ARM9 and ARM7 binaries from the cartridge into RAM. */
void Reset_ReloadProgramsFromCard(void)
{
    u32 base;
    u32 arm9Offset;
    u32 arm9Dest;
    u32 arm9Size;
    u32 arm7Offset;
    u32 arm7Dest;
    u32 arm7Size;
    base = ROM_IMAGE_BASE;

    if (base >= SECURE_AREA_END) {
        OSi_ReadCardRom32(base, ROM_HEADER_BUF, sizeof(RomHeader));
    }
    arm9Offset = ROM_HEADER_BUF->arm9RomOffset;
    arm9Dest = ROM_HEADER_BUF->arm9RamAddress;
    arm9Size = ROM_HEADER_BUF->arm9Size;
    arm7Offset = ROM_HEADER_BUF->arm7RomOffset;
    arm7Dest = ROM_HEADER_BUF->arm7RamAddress;
    arm7Size = ROM_HEADER_BUF->arm7Size;
    func_0210e0d4();
    func_0210e160();
    func_0210e154();
    arm7Offset += base;
    arm9Offset += base;
    if (arm9Offset < SECURE_AREA_END) {
        u32 skip = SECURE_AREA_END - arm9Offset;
        arm9Offset = SECURE_AREA_END;
        arm9Dest += skip;
        arm9Size -= skip;
    }
    OSi_ReadCardRom32(arm9Offset, (void *)arm9Dest, arm9Size);
    OSi_ReadCardRom32(arm7Offset, (void *)arm7Dest, arm7Size);
}
