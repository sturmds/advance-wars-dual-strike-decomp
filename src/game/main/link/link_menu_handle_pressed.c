#include "types.h"
#include "game/keypad.h"

void LinkMenu_Cancel(void);
void LinkMenu_Confirm(void);

/* A confirms (LinkMenu_Confirm), B cancels. */
void LinkMenu_HandlePressed(void)
{
    u16 keys = gKeypad->pressed;

    if (keys & PAD_BUTTON_A) {
        LinkMenu_Confirm();
    } else if (keys & PAD_BUTTON_B) {
        LinkMenu_Cancel();
    }
}
