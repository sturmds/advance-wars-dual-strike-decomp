#include "types.h"
#include "game/event_queue.h"
#include "game/keypad.h"
#include "game/menu_state.h"

u32 Link_IsActive(void);
int func_02064e00(int row);
void func_020651ec(void);
void func_02065214(void);
void VsSetup_IncreaseSetting(void);
void VsSetup_DecreaseSetting(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

#define SETTING_ROWS 9

/* Keys of the Vs setup settings menu: left and right pick the setting row
 * (setup.unk_2e6, wrapping), up and down step its value, A and B close the
 * menu (func_02065214 and func_020651ec, with different sounds). In link
 * play each action goes to the event queue instead, for
 * VsSetup_UpdateSettingsMenu to apply on every machine. */
void VsSetup_HandleSettingsKeys(void)
{
    s8 row;

    row = MENU->setup.unk_2e6;
    if (gKeypad->repeat & PAD_KEY_LEFT) {
        row--;
        if (row < 0) {
            row = SETTING_ROWS - 1;
        }
        if (Link_IsActive() == 1) {
            EventQueue_Push(EVENT_VS_SETTING_SELECT_ROW, (u8)row);
        } else {
            func_02064e00((u8)row);
        }
    }
    if (gKeypad->repeat & PAD_KEY_RIGHT) {
        row++;
        if (row >= SETTING_ROWS) {
            row = 0;
        }
        if (Link_IsActive() == 1) {
            EventQueue_Push(EVENT_VS_SETTING_SELECT_ROW, (u8)row);
        } else {
            func_02064e00((u8)row);
        }
    }
    if (gKeypad->repeat & PAD_KEY_UP) {
        if (Link_IsActive() == 1) {
            EventQueue_Push(EVENT_VS_SETTING_INCREASE, 0);
        } else {
            VsSetup_IncreaseSetting();
        }
    }
    if (gKeypad->repeat & PAD_KEY_DOWN) {
        if (Link_IsActive() == 1) {
            EventQueue_Push(EVENT_VS_SETTING_DECREASE, 0);
        } else {
            VsSetup_DecreaseSetting();
        }
    }
    if (gKeypad->pressed & PAD_BUTTON_A) {
        if (Link_IsActive() == 1) {
            EventQueue_Push(EVENT_VS_SETTINGS_CONFIRM, 0);
        } else {
            func_02065214();
        }
    } else if (gKeypad->pressed & PAD_BUTTON_B) {
        if (Link_IsActive() == 1) {
            EventQueue_Push(EVENT_VS_SETTINGS_CANCEL, 0);
        } else {
            func_020651ec();
        }
    }
}
