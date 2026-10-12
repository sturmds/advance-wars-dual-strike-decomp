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
void Power_SaveSyncResult(u32 result, u32 *dest);
int Power_SpinUntilIdle(void);
BOOL Power_LcdsPowered(void);
BOOL Power_SwitchLcds(int on, int backlight, BOOL force, BOOL sync);

/* Frame counter in shared main RAM, raised by the VBlank interrupt. */
#define SHARED_FRAME_COUNTER ((volatile u32 *)0x027ffc3c)

/* ARM7 side: the power chip sits on the SPI bus. */
#define REG_SPI_CNT ((volatile u16 *)0x040001c0)
#define REG_SPI_DATA ((volatile u16 *)0x040001c2)
#define SPI_BUSY 0x80

/* Set in the first SPI byte to read a power chip register instead of writing it. */
#define POWER_CHIP_READ_BIT 0x80

/* Power chip register 0 holds the control bits (LCD, backlights, sound, LED). */
#define POWER_SWITCHES_REG_INDEX 0

/* Sets up SPICNT for the power chip; continuous holds chip select for another byte. */
int func_arm7_038052e8(int continuous);
u8 PMi_GetRegister(int reg);
void PMi_SetRegister(int reg, int value);
void PMi_SetControl(int bits);
void PMi_ResetControl(int bits);

#endif
