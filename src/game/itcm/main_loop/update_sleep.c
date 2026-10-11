#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

int Link_IsActive(void);
int func_020fcad8(void);
void func_01ff94e4(void);
void func_01ff94fc(void);

/* Key state the ARM7 writes to shared memory; bit 15 is set while the lid is closed. */
#define SHARED_KEYS (*(volatile u16 *)0x027fffa8)
#define SHARED_KEYS_LID_BIT 0x8000

/* Closing the lid starts a sleep: state 2 waits for the link to finish before func_01ff94e4 sleeps. */
void MainLoop_UpdateSleep(void)
{
    switch (gMainLoop.unk_24) {
    case 0:
        return;
    case 1:
        if (((SHARED_KEYS & SHARED_KEYS_LID_BIT) >> 15) == 0) {
            return;
        }
        func_01ff94fc();
        return;
    case 2:
        if (Link_IsActive() != 0) {
            return;
        }
        if (func_020fcad8() != 0) {
            return;
        }
        func_01ff94e4();
        gMainLoop.unk_24 = 1;
        return;
    }
}
