#ifndef SDK_PM_H
#define SDK_PM_H

#include "types.h"

/* One entry of a sleep callback list (pre-sleep or post-sleep). */
typedef struct PowerHookEntry PowerHookEntry;
struct PowerHookEntry {
    void (*callback)(void *arg);
    void *arg;
    PowerHookEntry *next;
};

/* Frame counter in shared main RAM, raised by the VBlank interrupt. */
#define SHARED_FRAME_COUNTER ((volatile u32 *)0x027ffc3c)

#endif
