#ifndef SDK_PXI_H
#define SDK_PXI_H

#include "types.h"

/* PXI: the word FIFO between the two CPUs. Every library that talks to the
 * ARM7 sends 32-bit messages through it and registers a receive callback per
 * 5-bit tag. */

#define IO_FIFO_CTRL  ((volatile u16 *)0x04000184)
#define IO_FIFO_OUTBOX ((volatile u32 *)0x04000188)
#define IO_FIFO_INBOX ((volatile u32 *)0x04100000)

#define FIFO_CTRL_OUTBOX_FULL  0x0002
#define FIFO_CTRL_INBOX_EMPTY 0x0100
#define FIFO_CTRL_FAULT_ACK      0x4000
#define FIFO_CTRL_ON     0x8000

#define FIFO_PUSH_FAULT (-1)            /* FIFO error flag was set */
#define FIFO_PUSH_NO_ROOM  (-2)            /* send FIFO full */

#define FIFO_TAG_LIMIT 32

/* A FIFO message: tag in the low 5 bits, an error bit, then 26 bits of data. */
typedef struct PXIFifoMessage {
    u32 tag : 5;
    u32 err : 1;
    u32 data : 26;
} PXIFifoMessage;

typedef void (*PXIFifoCallback)(u32 tag, u32 data, int err);

/* Shared main memory at 0x027ffc00: bit n of word p says that CPU p has a
 * receive callback registered for tag n. */
typedef struct FifoPeerRegistry {
    u8 pad_000[0x388];
    u32 callbackFlags[2];              /* 0x388 */
} FifoPeerRegistry;

#define FIFO_PEER_REGISTRY ((volatile FifoPeerRegistry *)0x027ffc00)

#define PXI_PROC_ARM9 0
#define PXI_PROC_ARM7 1

int PXI_SendWordByFifo(u32 tag, u32 data, int err);
BOOL PXI_IsCallbackReady(u32 tag, u32 proc);
void PXI_SetFifoRecvCallback(u32 tag, PXIFifoCallback callback);

#endif
