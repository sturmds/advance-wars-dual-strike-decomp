#ifndef SDK_ISD_H
#define SDK_ISD_H

#include "types.h"

/* Status words the IS-NITRO debugger library shares with the debugger, in main memory. */
typedef struct IsdSharedBlock {
    u32 unk_00[5];
    u32 flags;
} IsdSharedBlock;

#define ISD_SHARED ((volatile IsdSharedBlock *)0x027fff60)

BOOL Isd_TestFlags(u32 mask);

/* The cartridge bus setting saved before the library uses the debugger's buffers, and put back afterwards. */
typedef struct IsdBusState {
    u16 exmemcnt;
    u16 sharedWord;
    u16 active;
} IsdBusState;

/* A halfword in the shared block that holds the debugger's current mode. */
#define ISD_SHARED_MODE (*(vu16 *)0x027fff72)

/* Debugger buffer on the cartridge bus: a control halfword and a register that is read and discarded. */
#define ISD_BUFFER_CONTROL (*(vu16 *)0x09fe2ffe)
#define ISD_BUFFER_STATUS (*(vu16 *)0x09fe20f0)

/*
 * Reads one byte with a halfword load. The library does this for every byte
 * it reads, because some of the memory it uses (the debugger's buffers on the
 * cartridge bus) takes only 16-bit accesses.
 */
static inline int Isd_ReadByte(const char *address)
{
    if ((u32)address & 1) {
        return (*(u16 *)(address - 1) & 0xff00) >> 8;
    }
    return *(u16 *)address & 0xff;
}

#endif
