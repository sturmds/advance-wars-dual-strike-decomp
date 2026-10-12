#ifndef SDK_CARD_H
#define SDK_CARD_H

#include "types.h"
#include "sdk/os.h"
#include "sdk/card_common.h"

typedef struct CARDiCommon CARDiCommon;
typedef void (*GameCardTaskStep)(CARDiCommon *common);

/* ARM9 CARD library state, data_022abb00 (0x5e0 bytes with the thread stack). */
struct CARDiCommon {
    CARDiCommandArg *cmd;
    u32 unk_04;
    volatile int lockOwner;        /* OS lock id of the owner, OS_LOCK_ID_ERROR when free; volatile is needed to match */
    int lockCount;
    OSThreadQueue lockQueue;
    u32 lockTarget;
    u32 src;
    u32 dst;
    u32 len;
    u32 unk_24;
    CARDCallback callback;
    void *callbackArg;
    GameCardTaskStep task;
    u32 flag;
    u32 priorityMode;              /* thread priority, or GAMECARD_PRIO_* */
    OSThread thread;
    OSThread *waiter;              /* thread to wake when the request ends */
    OSThreadQueue queue;           /* threads waiting for CARD_STAT_BUSY to clear */
    u8 unk_D8[8];
    u8 pageBuffer[GAMECARD_BOUNCE_BUF_BYTES];
    u8 stack[GAMECARD_WORKER_STACK_BYTES];
};

extern CARDiCommon data_022abb00;

/* Set once the other CPU reports the card removed, and the game's handler for that. */
extern BOOL data_022ac320;
extern BOOL (*data_022ac324)(void);

extern CARDiRomPage data_022ac100;

#endif
