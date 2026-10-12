#ifndef SDK_WMSP_H
#define SDK_WMSP_H

#include "types.h"
#include "sdk/wm.h"

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
} WmspBuf;

/* What the ARM9 sends for a request that carries one value. */
typedef struct WmspRequest {
    u16 apiid;
    u8 pad_02[2];
    u32 arg;
} WmspRequest;

typedef struct WmspWork {
    u8 unk_0000[0x1518];
    WmspBuf *buf;
} WmspWork;

WmspWork *func_arm7_027c05e0(void);
void func_arm7_027c73a4(void);
int func_arm7_027c75fc(u16 *a, u16 *b);
void func_arm7_03805688(int arg);
void func_arm7_037fd9dc(void *object);
void func_arm7_027c6eb4(u16 arg);
void func_arm7_027c6f68(u16 arg);
void func_arm7_027c70f8(void);
WmspReply *func_arm7_03807640(void);
void func_arm7_038076e8(void);

/* Copies halfwords (ARM7's MI_CpuCopy16-like helper). */
void func_arm7_037fde40(const void *src, void *dest, int size);

/* Sends the command to the firmware. */
void func_arm7_027c0560(WlCommand *command);

#endif
