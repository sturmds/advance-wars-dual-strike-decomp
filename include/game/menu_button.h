#ifndef GAME_MENU_BUTTON_H
#define GAME_MENU_BUTTON_H

#include "types.h"

/* One selectable entry of a menu screen, 0x3c bytes. The game keeps 80
 * of them in gMenuButtons; MenuButton_Find finds one by id, and
 * MenuButton_Select makes it the selected one. While a button is
 * selected, Menu_DispatchKeys calls its onKeys when a key in
 * Keypad.repeat is down. */
typedef struct MenuButton {
    u8 id;                             /* menus use it as the cursor position */
    u8 active;
    u8 pad_02[0x1c - 0x02];
    u16 unk_1c;                        /* flag bits, read by func_02026404 */
    u8 pad_1e[0x28 - 0x1e];
    void (*onKeys)(struct MenuButton *button);
    void (*onConfirm)(struct MenuButton *button);  /* called by MenuButton_Confirm */
    void (*onCancel)(struct MenuButton *button);   /* called by Menu_CancelSelected on the selected button */
    void (*onSelect)(struct MenuButton *button);   /* called when the button becomes the selected one */
    u8 pad_38[0x3c - 0x38];
} MenuButton;

extern MenuButton gMenuButtons[80];

/* Menu button state reached through data_02158f38. */
typedef struct MenuButtonState {
    u8 pad_00[0x08];
    u8 unk_08;                         /* set together with selected by func_0202621c */
    u8 pad_09[0x0c - 0x09];
    MenuButton *selected;
} MenuButtonState;

extern MenuButtonState *data_02158f38;

MenuButton *MenuButton_Find(int id);           /* active button with this id, or NULL */
MenuButton *MenuButton_Select(int id);
void MenuButton_Confirm(MenuButton *button);
void Menu_CancelSelected(void);
void MenuButton_NotifySelected(MenuButton *button);

#endif
