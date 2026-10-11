#ifndef SDK_ISD_H
#define SDK_ISD_H

#include "types.h"

/* Status words the IS-NITRO debugger library shares with the debugger, in main memory. */
typedef struct IsdSharedBlock {
    u32 unk_00[5];
    u32 flags;
    u32 unk_18;
    u8 *channel;
} IsdSharedBlock;

/* Ring of output bytes inside the channel (ring at channel + 0x90, bytes at channel + 0x8000). */
typedef struct IsdRing {
    u16 head;
    u16 unk_02;
    u16 tail;
} IsdRing;

#define ISD_RING_BYTES_OFFSET 0x8000
#define ISD_RING_MASK 0x3fff

/* The cartridge-bus ring is 32 KB (the channel's is 16 KB); which window of the bus it sits in depends on a flag. */
#define ISD_CART_RING_MASK 0x7fff
#define ISD_FLAG_CART_HIGH_BUFFER 0x2000
#define ISD_CART_BUFFER_HIGH 0x09fd0000
#define ISD_CART_BUFFER_LOW 0x08fd0000

#define ISD_SHARED ((volatile IsdSharedBlock *)0x027fff60)

BOOL Isd_TestFlags(u32 mask);

/* The cartridge bus setting saved before the library uses the debugger's buffers, and put back afterwards. */
typedef struct IsdBusState {
    u16 exmemcnt;
    u16 sharedWord;
    u16 active;
} IsdBusState;

typedef struct IsdPrintTarget IsdPrintTarget;

/* Where formatted text goes: the formatter calls write with pieces of the output. */
struct IsdPrintTarget {
    u32 unk_00;
    u32 limit;
    u32 total;
    u32 stored;
    void (*write)(IsdPrintTarget *target, const char *text, int length);
};

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

/* Writes one byte at bytes[index] with a halfword read and write, for the same reason as Isd_ReadByte. */
static inline void Isd_WriteByte(u8 *bytes, u32 index, int value)
{
    u32 offset = index & ~1;
    u16 half = *(u16 *)(bytes + offset);

    if (index & 1) {
        half = (half & 0x00ff) | ((u16)value << 8);
    } else {
        half = (half & 0xff00) | (u16)value;
    }
    *(u16 *)(bytes + offset) = half;
}

#endif
