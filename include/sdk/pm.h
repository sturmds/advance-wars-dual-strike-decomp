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

/* Called with the ARM7 result and the argument given when the command was started. */
typedef void (*PMCallback)(u32 result, u32 *arg);

/* The command in flight: PM runs one at a time (lock), and the reply calls callback. */
typedef struct PowerCommandState PowerCommandState;
struct PowerCommandState {
    int lock;
    PMCallback callback;
    u32 *arg;
    u32 *unk_0c;
};

/* Per PM register: whether its read finished, and where to store the value. */
typedef struct PowerChipReadSlot PowerChipReadSlot;
struct PowerChipReadSlot {
    u16 done;
    u16 *data;
};

#define POWER_CHIP_READ_SLOTS 4

void PowerHook_CallAll(PowerHookEntry *list);
BOOL Power_LcdsPowered(void);
BOOL Power_SwitchLcds(int on, int backlight, BOOL force, BOOL sync);

/* Frame counter in shared main RAM, raised by the VBlank interrupt. */
#define SHARED_FRAME_COUNTER ((volatile u32 *)0x027ffc3c)

#endif
