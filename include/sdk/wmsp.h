#ifndef SDK_WMSP_H
#define SDK_WMSP_H

#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"

// WMSP is the ARM7 half of WM. It runs the requests the ARM9 sends and
// reports back through a reply buffer that both CPUs can see.

/* Wireless hardware configuration registers, named by address (GBATEK). */
#define IO_WIRELESS_INIT_REG_124 ((volatile u16 *)0x04808124)
#define IO_WIRELESS_INIT_REG_128 ((volatile u16 *)0x04808128)
#define IO_WIRELESS_INIT_REG_150 ((volatile u16 *)0x04808150)

#define WMSP_SLOT_COUNT 32
#define WMSP_SLOT_FLAG 0x8000

typedef struct WmspSlot {
    u32 flags;
    u8 pad_04[12];
} WmspSlot;

/* ARM7 side of WM: a command for the wireless (WL) firmware.
   words[] holds the arguments, then the reply header at words[index]:
   the argument count, then the command id. WL fills in the reply. */
typedef struct WlCommand {
    u16 reserved[6];
    u16 length;
    u16 index;
    u16 words[1];
} WlCommand;

/* The ARM9 asks the ARM7 to start WM with these pointers (the arguments of
   WM_Initialize's request). */
typedef struct WmspInitRequest {
    u16 apiid;
    u8 pad_02[2];
    void *arm7Buf;
    WMStatus *status;
    void *replyBuf;
} WmspInitRequest;

/* What the ARM7 writes into the reply buffer (the start of WMCallback). */
typedef struct WmspReply {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
} WmspReply;

/* The ARM7 side's work area, reached through the pointer at its end. */
typedef struct WmspBuf {
    WMStatus *status;
    u8 pad_04[4];
    WmspReply *replyBuf;
    u8 pad_0c[0xc4];
    u8 unk_d0[0x24];                   /* three words are cleared by func_arm7_037fd9dc */
    u8 pad_f4[4];
    WmspSlot slots[WMSP_SLOT_COUNT];
} WmspBuf;

/* What the ARM9 sends for a request that carries one value. */
typedef struct WmspRequest {
    u16 apiid;
    u8 pad_02[2];
    u32 arg;
} WmspRequest;

/* What the ARM9 sends for a request about one station. */
typedef struct WmspMacRequest {
    u16 apiid;
    u8 pad_02[2];
    u8 macAddress[6];
} WmspMacRequest;

// The symbol data_arm7_027d7c44, which is the same memory as WmspWork from
// 0x1000 on. Only the part WMSP uses.
typedef struct WmspState {
    u8 pad_000[0x50c];
    u32 unk_50c;
    u32 unk_510;
    u32 unk_514;
    WmspBuf *buf;
    u8 history[16];
    volatile u32 historyIndex;  /* volatile: needed to match, the original reloads it */
    u32 threadPriority[2][3];  /* [1] is what the WMSP threads were created with */
} WmspState;

// The work area starts 0x1000 bytes before data_arm7_027d7c44.
#define WMSP_STATE_OFFSET 0x1000

typedef struct WmspWork {
    OSMessageQueue wlQueue;            /* commands for the WL driver */
    OSMessage wlMessages[2];
    u8 pad_1c[0x1518 - 0x1c];
    WmspBuf *buf;
} WmspWork;

#define WMSP_WORK ((WmspWork *)0x027d6c44)

extern WmspState data_arm7_027d7c44;

/* The parent parameters kept in WMStatus (see WMStatus.pparam). */
static inline WMParentParam *Wmsp_ParentParam(WMStatus *status)
{
    return (WMParentParam *)status->pparam;
}

/* Held while a reply buffer is filled and sent. */
extern OSMutex data_arm7_027d813c;

/* Take the buffer for a reply to the ARM9, fill it, and send it. */
void *Wmsp_GetCallbackBuffer(void);
void Wmsp_SendReply(void *reply);

/* The same 16 bytes as WmspState.history; the original names them apart. */
extern u8 data_arm7_027d8160[16];

WmspWork *func_arm7_027c05e0(void);
void WMSP_ResetStatus(void);
int func_arm7_027c75fc(u16 *a, u16 *b);
void func_arm7_037fd9dc(void *object);
void func_arm7_027c6eb4(u16 arg);
void func_arm7_027c6f68(u16 arg);
void func_arm7_027c70f8(void);

/* Copies halfwords (ARM7's MI_CpuCopy16-like helper). */
void func_arm7_037fde40(const void *src, void *dest, int size);

/* Sends the command to the firmware and returns its answer. */
WlCommand *Wmsp_SendWlCommand(WlCommand *command);

#endif
