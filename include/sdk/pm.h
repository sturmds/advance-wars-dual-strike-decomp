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

/* Power chip register 0 holds the control bits below. */
#define POWER_SWITCHES_REG_INDEX 0

/* Bits of the control register. */
#define POWER_SWITCH_AMPLIFIER 0x01
#define POWER_SWITCH_SILENCE 0x02
#define POWER_SWITCH_LOWER_LAMP 0x04
#define POWER_SWITCH_UPPER_LAMP 0x08
#define POWER_SWITCH_LED_FLASHING 0x10
#define POWER_SWITCH_LED_QUICK_RATE 0x20
#define POWER_SWITCH_CUT_SUPPLY 0x40

/* Power LED states. */
#define PM_LED_ON 1
#define POWER_LIGHT_SLOW_FLASH 2
#define POWER_LIGHT_QUICK_FLASH 3

/* Power commands the ARM9 sends over PXI; the ARM7 applies them to the control register. */
#define POWER_ORDER_LED_STEADY 1
#define POWER_ORDER_LED_QUICK_FLASH 2
#define POWER_ORDER_LED_LAZY_FLASH 3
#define POWER_ORDER_LOWER_LCD_LIT 4
#define POWER_ORDER_LOWER_LCD_DARK 5
#define POWER_ORDER_UPPER_LCD_LIT 6
#define POWER_ORDER_UPPER_LCD_DARK 7
#define POWER_ORDER_BOTH_LCDS_LIT 8
#define POWER_ORDER_BOTH_LCDS_DARK 9
#define POWER_ORDER_AMPLIFIER_ENABLE 10
#define POWER_ORDER_AMPLIFIER_DISABLE 11
#define POWER_ORDER_SILENCE_SPEAKER 12
#define POWER_ORDER_UNSILENCE_SPEAKER 13
#define POWER_ORDER_SHUT_DOWN 14
#define POWER_ORDER_CANCEL_SHUTDOWN 15

/* PM PXI command that puts the system to sleep; the ARM7 acknowledges it on wake-up. */
#define POWER_MSG_ENTER_SLEEP 0x62

/* Other wake-up triggers for sleep mode. */
#define PM_TRIGGER_KEY 0x1
#define PM_TRIGGER_RTC_ALARM 0x2
#define PM_TRIGGER_COVER_OPEN 0x4

/* ARM7 registers and interrupt bits used to wake from sleep. */
#define REG_KEYCNT ((volatile u16 *)0x04000132)
#define REG_RCNT ((volatile u16 *)0x04000134)
#define KEYCNT_IRQ 0x4000
#define IRQ_RTC 0x80
#define IRQ_KEY 0x1000
#define IRQ_CARTRIDGE 0x2000
#define IRQ_CARD 0x100000
#define IRQ_COVER_OPEN 0x400000

/* PM commands from the ARM9 arrive here a halfword at a time. */
typedef struct PowerRequestInbox PowerRequestInbox;
struct PowerRequestInbox {
    u16 words[16];
    u32 unk_20;
};

/* Sets up SPICNT for the power chip; continuous holds chip select for another byte. */
int func_arm7_038052e8(int continuous);
u8 PMi_GetRegister(int reg);
void PMi_SetRegister(int reg, int value);
void PMi_SetControl(int bits);
void PMi_ResetControl(int bits);
void PMi_SetLED(int status);
void Power_ApplyArm9Command(int command);

/* Last state given to PMi_SetLED. */
extern int gPowerLedLastState;

#endif
