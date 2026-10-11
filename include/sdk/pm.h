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

/* PXI FIFO tag of the power management service. */
#define PXI_FIFO_TAG_PM 8

/* Wake-up triggers for sleep mode. */
#define PM_TRIGGER_CARD 0x8
#define PM_TRIGGER_CARTRIDGE 0x10

void PowerHook_CallAll(PowerHookEntry *list);
BOOL Power_LcdsPowered(void);
BOOL Power_SwitchLcds(int on, int backlight, BOOL force, BOOL sync);

/* Frame counter in shared main RAM, raised by the VBlank interrupt. */
#define SHARED_FRAME_COUNTER ((volatile u32 *)0x027ffc3c)

#endif
