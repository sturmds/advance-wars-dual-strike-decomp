#include "types.h"
#include "game/event_queue.h"
#include "game/keypad.h"
#include "game/menu_state.h"

u32 Link_IsActive(void);
int VsSetup_SelectSettingRow(int row);
void VsSetup_CancelSettingsMenu(void);
void VsSetup_CloseSettingsMenu(void);
void VsSetup_IncreaseSetting(void);
void VsSetup_DecreaseSetting(void);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

#define SETTING_ROWS 9

/* Keys of the Vs setup settings menu: left and right pick the setting row
 * (setup.settingRow, wrapping), up and down step its value, A and B close the
 * menu (VsSetup_CloseSettingsMenu and VsSetup_CancelSettingsMenu, with different sounds). In link
 * play each action goes to the event queue instead, for
 * VsSetup_UpdateSettingsMenu to apply on every machine. */
void VsSetup_HandleSettingsKeys(void)
{
    s8 row;

    row = MENU->setup.settingRow;
    if (gKeypad->repeat & PAD_KEY_LEFT) {
        row--;
        if (row < 0) {
            row = SETTING_ROWS - 1;
        }
        if (Link_IsActive() == 1) {
            EventQueue_Push(EVENT_VS_SETTING_SELECT_ROW, (u8)row);
        } else {
            VsSetup_SelectSettingRow((u8)row);
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
            VsSetup_SelectSettingRow((u8)row);
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
            VsSetup_CloseSettingsMenu();
        }
    } else if (gKeypad->pressed & PAD_BUTTON_B) {
        if (Link_IsActive() == 1) {
            EventQueue_Push(EVENT_VS_SETTINGS_CANCEL, 0);
        } else {
            VsSetup_CancelSettingsMenu();
        }
    }
}
