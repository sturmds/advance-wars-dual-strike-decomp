#include "sdk/gx.h"
#include "sdk/os.h"
#include "sdk/pm.h"

extern PowerHookEntry *data_022ab8f0; /* pre-sleep callbacks */
extern PowerHookEntry *data_022ab8f8; /* post-sleep callbacks */

void func_0210eb70(void);
BOOL Cartridge_HeaderStillMatches(void);
int PM_GetBackLight(int *topBackLight, int *bottomBackLight);
int PM_ForceToPowerOff(void);
int PM_SetBackLight(int target, int value);
void func_02114ee8(int backlight);
int func_021150b4(u16 trigger, u16 keys);

#define IRQ_CARD_IREQ 0x100000

static inline u16 SetIme(u16 value)
{
    u16 previous = *REG_IME;
    *REG_IME = value;
    return previous;
}

void func_02114ae8(u32 trigger, u16 logic, u16 keyPattern)
{
    u16 savedIme;
    volatile int savedInterrupts; /* volatile: needed to match (stack slot) */
    u16 command;
    u32 savedDispcnt;
    u32 savedSubDispcnt;
    u32 now;
    vu32 savedIrqMask;
    volatile BOOL lcdWasOn;
    int topBackLight;
    BOOL cardEjected;
    int bottomBackLight;
    vu32 tick;

    cardEjected = FALSE;

    PowerHook_CallAll(data_022ab8f0);
    savedIme = SetIme(0);
    savedInterrupts = OS_DisableInterrupts();
    savedIrqMask = OS_DisableIrqMask(0x3fffff);
    OS_SetIrqMask(IRQ_FIFO_RECV);
    OS_RestoreInterrupts(savedInterrupts);
    SetIme(1);

    if (trigger & PM_TRIGGER_CARD) {
        if (*BOOT_ORIGIN_WORD == BOOT_ORIGIN_WIRELESS) {
            trigger &= ~PM_TRIGGER_CARD;
        }
    }
    if (trigger & PM_TRIGGER_CARTRIDGE) {
        if (!Cartridge_HeaderStillMatches()) {
            trigger &= ~PM_TRIGGER_CARTRIDGE;
        }
    }

    savedDispcnt = *REG_DISPCNT;
    savedSubDispcnt = *REG_DB_DISPCNT;
    lcdWasOn = Power_LcdsPowered();
    PM_GetBackLight(&topBackLight, &bottomBackLight);
    PM_SetBackLight(2, 0);

    tick = *SHARED_FRAME_COUNTER;
    do {
        now = *SHARED_FRAME_COUNTER;
    } while (tick == now);
    tick = *SHARED_FRAME_COUNTER;
    GX_DispOff();
    *REG_DB_DISPCNT &= ~0x10000;
    do {
        now = *SHARED_FRAME_COUNTER;
    } while (tick == now);
    tick = *SHARED_FRAME_COUNTER;
    do {
        now = *SHARED_FRAME_COUNTER;
    } while (tick == now);

    logic = logic | keyPattern;
    command = trigger | (topBackLight << 5) | (bottomBackLight << 6);
    while (func_021150b4(command, logic) != 0) {
    }

    func_0210eb70();
    OS_SpinWait(0x332340);

    if (trigger & PM_TRIGGER_CARD) {
        if (*REG_IF & IRQ_CARD_IREQ) {
            cardEjected = TRUE;
        }
    }
    if (!cardEjected) {
        if (lcdWasOn == TRUE) {
            Power_SwitchLcds(1, 1, 1, 0);
            GX_DispOn();
        } else {
            func_02114ee8(1);
        }
        *REG_DISPCNT = savedDispcnt;
        *REG_DB_DISPCNT = savedSubDispcnt;
    }

    OS_DisableInterrupts();
    OS_SetIrqMask(savedIrqMask);
    OS_RestoreInterrupts(savedInterrupts);
    SetIme(savedIme);
    if (cardEjected) {
        PM_ForceToPowerOff();
    }
    PowerHook_CallAll(data_022ab8f8);
}
