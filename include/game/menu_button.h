#ifndef GAME_MENU_BUTTON_H
#define GAME_MENU_BUTTON_H

#include "types.h"

/* One selectable entry of a menu screen, 0x3c bytes. The game keeps 80 of
 * them at gMenuButtons; func_02025d58 finds one by id, and func_020262b0
 * makes it the selected one. While a button is selected, func_0202617c calls
 * its onKeys when a key in Keypad.repeat is down. */
typedef struct MenuButton {
    u8 id;                             /* menus use it as the cursor position */
    u8 active;
    u8 pad_02[0x1c - 0x02];
    u16 unk_1c;                        /* flag bits, read by func_02026404 */
    u8 pad_1e[0x28 - 0x1e];
    void (*onKeys)(struct MenuButton *button);
    void (*onConfirm)(struct MenuButton *button);  /* called by func_020250a8 */
    void (*onCancel)(struct MenuButton *button);   /* called by func_02025008 on the selected button */
    u8 pad_34[0x3c - 0x34];
} MenuButton;

extern MenuButton gMenuButtons[80];

#endif
