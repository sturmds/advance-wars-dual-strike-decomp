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
    u8 pad_02[0x03 - 0x02];
    u8 touchState;                     /* read and written by func_020263f4 and func_020263fc */
    u8 slideState;                     /* MENU_BUTTON_SLIDE_* */
    u8 pad_05[0x06 - 0x05];
    s16 slideFromX;
    s16 slideFromY;
    s16 slideToX;
    s16 slideToY;
    s16 slideTimer;                    /* counts down from slideDuration */
    u8 pad_10[0x14 - 0x10];
    s16 x;                             /* screen position in pixels */
    s16 y;
    s16 width;
    s16 height;
    u16 unk_1c;                        /* flag bits, read by func_02026404 */
    u16 slideDuration;                 /* frames */
    u16 tile;                          /* OAM character name of the sprite */
    u16 palette;                       /* OAM palette number */
    void (*onUpdate)(struct MenuButton *button);   /* called every frame by func_020257f8 */
    void (*onKeys)(struct MenuButton *button);
    void (*onConfirm)(struct MenuButton *button);  /* called by MenuButton_Confirm */
    void (*onCancel)(struct MenuButton *button);   /* called by Menu_CancelSelected on the selected button */
    void (*onSelect)(struct MenuButton *button);   /* called when the button becomes the selected one */
    void (*onSlideOutDone)(struct MenuButton *button);
} MenuButton;

/* Bits of MenuButton.unk_1c, tested with func_02026404. */
#define MENU_BUTTON_VISIBLE          0x0002
#define MENU_BUTTON_SLIDE_IN_LEFT    0x0004   /* slides in from off the left edge */
#define MENU_BUTTON_SLIDE_IN_RIGHT   0x0008
#define MENU_BUTTON_SLIDE_IN_TOP     0x0010
#define MENU_BUTTON_SLIDE_IN_BOTTOM  0x0020
#define MENU_BUTTON_SLIDE_OUT_LEFT   0x0040   /* slides out past the left edge */
#define MENU_BUTTON_SLIDE_OUT_RIGHT  0x0080
#define MENU_BUTTON_SLIDE_OUT_TOP    0x0100
#define MENU_BUTTON_SLIDE_OUT_BOTTOM 0x0200
#define MENU_BUTTON_SEMI_TRANSPARENT 0x0800

/* Values of MenuButton.slideState. */
enum {
    MENU_BUTTON_SLIDE_IN_START,
    MENU_BUTTON_SLIDING_IN,
    MENU_BUTTON_SHOWN,
    MENU_BUTTON_SLIDE_OUT_START,
    MENU_BUTTON_SLIDING_OUT,
    MENU_BUTTON_HIDDEN
};

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
void Menu_ConfirmSelected(void);
void MenuButton_NotifySelected(MenuButton *button);
void MenuButton_Draw(MenuButton *button);

#endif
