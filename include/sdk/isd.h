#ifndef SDK_ISD_H
#define SDK_ISD_H

#include "types.h"

/* Status words the IS-NITRO debugger library shares with the debugger, in main memory. */
typedef struct IsdSharedBlock {
  u32 unk_00[5];
  u32 flags;
  u32 unk_18;
  u8* channel;
} IsdSharedBlock;

/* Ring of output bytes inside the channel (ring at channel + 0x90, bytes at channel + 0x8000). */
typedef struct IsdRing {
  u16 head;
  u16 unk_02;
  u16 tail;
} IsdRing;

#define ISD_RING_OFFSET 0x90
#define ISD_RING_BYTES_OFFSET 0x8000
#define ISD_RING_MASK 0x3fff

/* Flag bits in the shared block: debugger output is on, and which ring carries it. */
#define ISD_FLAG_CART_RING 0x1
#define ISD_FLAG_CHANNEL_RING 0x4
#define ISD_FLAG_OUTPUT 0x20000

typedef void (*IsdByteWriter)(IsdRing* ring, int byte);

/* The cartridge-bus ring is 32 KB (the channel's is 16 KB); which window of the bus it sits in depends on a flag. */
#define ISD_CART_RING_MASK 0x7fff
#define ISD_FLAG_CART_HIGH_BUFFER 0x2000
#define ISD_CART_BUFFER_HIGH 0x09fd0000
#define ISD_CART_BUFFER_LOW 0x08fd0000

#define ISD_SHARED ((volatile IsdSharedBlock*)0x027fff60)

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
  void (*write)(IsdPrintTarget* target, const char* text, u32 length);
};

void Isd_PrintTargetWrite(IsdPrintTarget* target, const char* text, u32 length);

void Isd_Printf(const char* format, ...);

/* A halfword in the shared block that holds the debugger's current mode. */
#define ISD_SHARED_MODE (*(vu16*)0x027fff72)

/* Debugger buffer on the cartridge bus: a control halfword and a register that is read and discarded. */
#define ISD_BUFFER_CONTROL (*(vu16*)0x09fe2ffe)
#define ISD_BUFFER_STATUS (*(vu16*)0x09fe20f0)

void Isd_CartRingWriteByte(IsdRing* ring, int byte);
void Isd_BusSave(IsdBusState* saved);
void Isd_BusRestore(IsdBusState* saved);

/* Taking the cartridge bus for the debugger: *saved gets what Isd_ReleaseCartridge needs to undo it. */
BOOL Isd_TryAcquireCartridge(u32* saved);
void Isd_AcquireCartridge(u32* saved);
void Isd_ReleaseCartridge(u32 saved);

/*
 * Reads one byte with a halfword load. The library does this for every byte
 * it reads, because some of the memory it uses (the debugger's buffers on the
 * cartridge bus) takes only 16-bit accesses.
 */
static inline int Isd_ReadByte(const char* address) {
  if ((u32)address & 1) {
    return (*(u16*)(address - 1) & 0xff00) >> 8;
  }
  return *(u16*)address & 0xff;
}

/* Writes one byte at bytes[index] with a halfword read and write, for the same reason as Isd_ReadByte. */
static inline void Isd_WriteByte(u8* bytes, u32 index, int value) {
  u32 offset = index & ~1;
  u16 half = *(u16*)(bytes + offset);

  if (index & 1) {
    half = (half & 0x00ff) | ((u16)value << 8);
  } else {
    half = (half & 0xff00) | (u16)value;
  }
  *(u16*)(bytes + offset) = half;
}

/* Interrupt masks and CP15 protection unit access (hand-written assembly in isd_irq.c, isd_fiq.c, isd_cp15.c). */
BOOL Isd_DisableIrq(void);
BOOL Isd_RestoreIrq(BOOL enable);
BOOL Isd_DisableFiq(void);
BOOL Isd_RestoreFiq(BOOL enable);
u32 Isd_GetProtectionRegion7(void);
void Isd_SetProtectionRegion7(u32 value);
u32 Isd_GetDataPermissions(void);
void Isd_SetDataPermissions(u32 value);
u32 Isd_GetInstructionPermissions(void);
void Isd_SetInstructionPermissions(u32 value);

/* Access permission bits for protection region 7 in the CP15 permission registers; 3 is read and write. */
#define ISD_REGION7_ACCESS_MASK 0xc000
#define ISD_REGION7_ACCESS_READ_WRITE 0xc000

/* What Isd_OpenDebugRegion saves and Isd_CloseDebugRegion puts back. */
typedef struct IsdRegionState {
  BOOL irq;
  BOOL fiq;
  u32 region7;
  u32 instructionPermissions;
  u32 dataPermissions;
} IsdRegionState;

void Isd_OpenDebugRegion(IsdRegionState* saved);
void Isd_CloseDebugRegion(IsdRegionState* saved);

void Isd_ReportFailure(const char* file, int line, const char* expression,
                       BOOL isFatal);

/*
 * The table of loaded overlays the library keeps for the debugger (isdoverlay.c).
 * The field names come from the library's own assertion strings (m_VMA, m_SIZE,
 * m_header.m_nInfoCount).
 */
typedef struct IsdOverlayEntry {
  u32 vma;
  u32 fileOffset; /* moves with vma when an entry is trimmed at the front */
  u32 size;
  u32 unk_0C;
} IsdOverlayEntry;

typedef struct IsdOverlayHeader {
  u32 infoCount;
  u32 lock; /* taken with Isd_SwapWord(ISD_OVERLAY_LOCKED, &lock), freed by storing 0 */
  u32 flags;
  u32 unk_0C;
} IsdOverlayHeader;

#define ISD_OVERLAY_LOCKED 0x80
/* Set in flags while the table is being changed. */
#define ISD_OVERLAY_FLAG_UPDATING 0x2

u32 Isd_SwapWord(u32 value, u32* address);

typedef struct IsdOverlayInfo {
  IsdOverlayHeader header;
  IsdOverlayEntry info[];
} IsdOverlayInfo;

/* "isdoverlay.c", the file name in the overlay table's assertions, and the assertion texts the table code shares. */
extern const char gIsdOverlayFileName[];
extern const char gIsdEntryStartCheck[];
extern const char gIsdEntryEndCheck[];
extern const char gIsdEntryAtAddressCheck[];
extern const char gIsdWrapSizeCheck[];

void Isd_OverlayTableRemove(IsdOverlayInfo* table, u32 index);
void Isd_OverlayTableInsert(IsdOverlayInfo* table, int index);

/*
 * Set when overlay changes go to handler functions instead of the library's own
 * table code. Nothing in the ROM writes it or the handler pointers; the debugger
 * presumably does.
 */
extern BOOL gIsdOverlayHooksEnabled;

/* One overlay table per processor (MIProcessor: 0 ARM9, 1 ARM7); NULL when there is none. */
extern IsdOverlayInfo* gIsdOverlayTables[];

#endif
