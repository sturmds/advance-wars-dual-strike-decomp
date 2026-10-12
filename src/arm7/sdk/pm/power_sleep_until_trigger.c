#include "types.h"
#include "sdk/os.h"
#include "sdk/pm.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
u32 func_arm7_037fb654(u32 mask);
u32 func_arm7_037fb690(u32 mask);
u32 func_arm7_037fb6c8(u32 mask);
void func_arm7_037fe3e0(int mode);
void func_arm7_037fe3fc(int mask, int value);
void SndHw_Enable(void);
void SndHw_Disable(void);
void func_arm7_0380513c(int command, int arg, int data);
void func_arm7_0380566c(void);

extern u16 data_arm7_03809ff0;
extern u16 data_arm7_03809ff4;
extern PowerRequestInbox data_arm7_03809ffc;

/* IME writes that return the old value, as the SDK's IRQ helpers do. */
static inline u16 SetIme(u16 value)
{
    u16 prev = *REG_IME;
    *REG_IME = value;
    return prev;
}

/* Request bits besides the WAKE_ON_* ones: which backlights to turn on after waking. */
#define SLEEP_BACKLIGHT_TOP 0x20
#define SLEEP_BACKLIGHT_BOTTOM 0x40

/* RCNT in general-purpose mode, with the SI pin's direction bit and interrupt enable: the RTC alarm comes in on SI. */
#define RCNT_MODE_GPIO 0x8000
#define RCNT_SI_DIRECTION 0x40
#define RCNT_SI_IRQ 0x100

#define IRQ_ALL 0x01ffffff

/*
 * Sleeps until one of the requested triggers fires. data_arm7_03809ff4 holds the
 * triggers and backlight bits of the request, data_arm7_03809ff0 the key pattern.
 */
void Power_SleepUntilTrigger(void)
{
    u16 ime;
    int intr;
    u32 irqMask;
    u8 control;
    u16 rcnt;
    int topCommand;
    int bottomCommand;

    ime = SetIme(0);
    intr = func_arm7_037fdb80();
    irqMask = func_arm7_037fb654(IRQ_ALL);
    control = PMi_GetRegister(POWER_SWITCHES_REG_INDEX);

    PowerLed_SelectPattern(POWER_LIGHT_SLOW_FLASH);
    PMi_SetLED(POWER_LIGHT_SLOW_FLASH);
    PMi_SetLED(POWER_LIGHT_SLOW_FLASH);
    SndHw_Disable();
    PMi_ResetControl(POWER_SWITCH_AMPLIFIER);

    if (data_arm7_03809ff4 & PM_TRIGGER_KEY) {
        *REG_KEYCNT = data_arm7_03809ff0 | KEYCNT_IRQ;
        func_arm7_037fb690(IRQ_KEY);
    }
    if (data_arm7_03809ff4 & PM_TRIGGER_COVER_OPEN) {
        func_arm7_037fb690(IRQ_COVER_OPEN);
    }
    if (data_arm7_03809ff4 & PM_TRIGGER_RTC_ALARM) {
        rcnt = *REG_RCNT;
        func_arm7_037fe3e0(RCNT_MODE_GPIO);
        func_arm7_037fe3fc(RCNT_SI_DIRECTION, RCNT_SI_DIRECTION);
        func_arm7_037fe3fc(RCNT_SI_IRQ, RCNT_SI_IRQ);
        func_arm7_037fb690(IRQ_RTC);
    }
    if (data_arm7_03809ff4 & PM_TRIGGER_CARD) {
        func_arm7_037fb690(IRQ_CARD);
    }
    if (data_arm7_03809ff4 & PM_TRIGGER_CARTRIDGE) {
        func_arm7_037fb690(IRQ_CARTRIDGE);
    }

    func_arm7_037fdb94(intr);
    SetIme(1);
    func_arm7_0380566c();

    PMi_SetRegister(POWER_SWITCHES_REG_INDEX, control);
    topCommand = (data_arm7_03809ff4 & SLEEP_BACKLIGHT_TOP) ? POWER_ORDER_UPPER_LCD_LIT : POWER_ORDER_UPPER_LCD_DARK;
    bottomCommand = (data_arm7_03809ff4 & SLEEP_BACKLIGHT_BOTTOM) ? POWER_ORDER_LOWER_LCD_LIT : POWER_ORDER_LOWER_LCD_DARK;
    Power_ApplyArm9Command(topCommand);
    Power_ApplyArm9Command(bottomCommand);
    /* rcnt is only saved for an RTC wake-up, but the original restores it every time */
    *REG_RCNT = rcnt;
    PMi_SetControl(POWER_SWITCH_AMPLIFIER);
    SndHw_Enable();

    data_arm7_03809ffc.unk_20 = 0;
    func_arm7_0380513c(POWER_MSG_ENTER_SLEEP, 0, 0);

    func_arm7_037fdb80();
    func_arm7_037fb6c8(irqMask);
    func_arm7_037fdb94(intr);
    SetIme(ime);
}
