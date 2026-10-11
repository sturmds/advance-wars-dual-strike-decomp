#include "types.h"
#include "game/keypad.h"

void LinkMenu_Cancel(void);
void func_0207c8b8(void);

/* A confirms (func_0207c8b8), B cancels. */
void LinkMenu_HandlePressed(void)
{
    u16 keys = gKeypad->pressed;

    if (keys & PAD_BUTTON_A) {
        func_0207c8b8();
    } else if (keys & PAD_BUTTON_B) {
        LinkMenu_Cancel();
    }
}
