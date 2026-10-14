#ifndef GAME_MENU_BUTTON_H
#define GAME_MENU_BUTTON_H

#include "types.h"

/* One selectable entry of a menu screen, 0x3c bytes. The game keeps
 * MENU_BUTTON_COUNT of them in gMenuButtons; MenuButton_Find finds one by id, and
 * MenuButton_Select makes it the selected one. While a button is
 * selected, Menu_DispatchKeys calls its onKeys when a key in
 * Keypad.repeat is down. */
typedef struct MenuButton {
  u8 id; /* menus use it as the cursor position */
  u8 active;
  u8 pad_02[0x03 - 0x02];
  u8 touchState; /* MENU_BUTTON_TOUCH_*, read and written by MenuButton_GetTouchState and MenuButton_SetTouchState */
  u8 slideState; /* MENU_BUTTON_SLIDE_* */
  u8 pad_05[0x06 - 0x05];
  s16 slideFromX;
  s16 slideFromY;
  s16 slideToX;
  s16 slideToY;
  s16 slideTimer; /* counts down from slideDuration */
  void*
      owner; /* the menu task a pop-up menu button belongs to (PopupMenu_AddButtons) */
  s16 x; /* screen position in pixels */
  s16 y;
  s16 width;
  s16 height;
  u16 flags;         /* MENU_BUTTON_* bits */
  u16 slideDuration; /* frames */
  u16 tile;          /* OAM character name of the sprite */
  u16 palette;       /* OAM palette number */
  void (*onUpdate)(
      struct MenuButton* button); /* called every frame by MenuButtons_Update */
  void (*onKeys)(struct MenuButton* button);
  void (*onConfirm)(
      struct MenuButton* button); /* called by MenuButton_Confirm */
  void (*onCancel)(
      struct MenuButton*
          button); /* called by Menu_CancelSelected on the selected button */
  void (*onSelect)(
      struct MenuButton*
          button); /* called when the button becomes the selected one */
  void (*onSlideOutDone)(struct MenuButton* button);
} MenuButton;

/* Bits of MenuButton.flags, tested with MenuButton_TestFlags. */
#define MENU_BUTTON_SELECT_ON_TOUCH 0x0001 /* a touch confirm also selects it */
#define MENU_BUTTON_VISIBLE 0x0002
#define MENU_BUTTON_SLIDE_IN_LEFT 0x0004 /* slides in from off the left edge */
#define MENU_BUTTON_SLIDE_IN_RIGHT 0x0008
#define MENU_BUTTON_SLIDE_IN_TOP 0x0010
#define MENU_BUTTON_SLIDE_IN_BOTTOM 0x0020
#define MENU_BUTTON_SLIDE_OUT_LEFT 0x0040 /* slides out past the left edge */
#define MENU_BUTTON_SLIDE_OUT_RIGHT 0x0080
#define MENU_BUTTON_SLIDE_OUT_TOP 0x0100
#define MENU_BUTTON_SLIDE_OUT_BOTTOM 0x0200
#define MENU_BUTTON_NO_TOUCH_CONFIRM \
  0x0400 /* touch neither selects nor confirms it */
#define MENU_BUTTON_SEMI_TRANSPARENT 0x0800
#define MENU_BUTTON_TOUCH_ANYWHERE \
  0x1000 /* MenuButton_ContainsPoint counts any point as inside */

/* Values of MenuButton.slideState. */
enum {
  MENU_BUTTON_SLIDE_IN_START,
  MENU_BUTTON_SLIDING_IN,
  MENU_BUTTON_SHOWN,
  MENU_BUTTON_SLIDE_OUT_START,
  MENU_BUTTON_SLIDING_OUT,
  MENU_BUTTON_HIDDEN
};

/* Values of MenuButton.touchState. */
enum {
  MENU_BUTTON_TOUCH_NONE,
  MENU_BUTTON_TOUCH_PRESSED, /* touched and still on the button */
  MENU_BUTTON_TOUCH_HELD     /* held; releasing no longer confirms */
};

/* Slots in gMenuButtons; MenuButton_Add leaves the last one free. */
#define MENU_BUTTON_COUNT 80
extern MenuButton gMenuButtons[];

/* What a button is made from; copied into MenuButton from x on, with
 * x and y moved by the position passed to the maker. */
typedef struct MenuButtonTemplate {
  s16 x;
  s16 y;
  s16 width;
  s16 height;
  u16 flags; /* becomes MenuButton.flags */
  u16 slideDuration;
  u16 tile;
  u16 palette;
  void (*onUpdate)(MenuButton* button);
  void (*onKeys)(MenuButton* button);
  void (*onConfirm)(MenuButton* button);
  void (*onCancel)(MenuButton* button);
  void (*onSelect)(MenuButton* button);
  void (*onSlideOutDone)(MenuButton* button);
} MenuButtonTemplate;

MenuButton* MenuButton_Find(int id); /* active button with this id, or NULL */
MenuButton* MenuButton_Select(int id);
void MenuButton_Confirm(MenuButton* button);
void Menu_CancelSelected(void);
void Menu_ConfirmSelected(void);
void MenuButton_NotifySelected(MenuButton* button);
void MenuButton_Draw(MenuButton* button);
void MenuButton_UpdateSlide(MenuButton* button);
BOOL MenuButton_HandleTouch(MenuButton* button);

BOOL MenuButton_ContainsPoint(MenuButton* button, int x, int y);
MenuButton* MenuButton_Init(int x, int y, MenuButton* button,
                            const MenuButtonTemplate* template, int id);
MenuButton* MenuButton_AddReserved(int x, int y,
                                   const MenuButtonTemplate* template);
MenuButton* MenuButton_Add(int x, int y, const MenuButtonTemplate* template,
                           int id);
void MenuButtons_CloseAll(void);
void MenuButton_Close(int id);
void MenuButtons_EnableTouch(void);
void MenuButtons_DisableTouch(void);
void MenuButtons_RemoveSelectable(void);
void MenuButtons_RemoveAll(void);
void MenuButton_Remove(int id);
void MenuButton_Deactivate(MenuButton* button);
void MenuButton_EnableTouch(int id);

/* The menu cursor: four corner sprites that frame the selected button,
 * reached through gMenuCursor. */
typedef struct MenuCursor {
  u32 tile; /* sprite tile of the first corner; each next corner is 2 tiles on */
  u32 palette;   /* sprite palette */
  u8 selectedId; /* id of the selected button */
  u8 pad_09[3];
  MenuButton* selected;
  s16 corners
      [4]
      [2]; /* x, y of the top left, top right, bottom left and bottom right corner */
  u8 screen; /* screen the menu draws on, index into data_027c0288 */
} MenuCursor;

extern MenuCursor* gMenuCursor;

u8 Menu_GetSelectedId(void);
void MenuCursor_DrawAt(int x, int y, int width, int height);
MenuButton* MenuCursor_SetSelected(MenuButton* button, int id);
MenuButton* MenuCursor_SelectButton(MenuButton* button);
s16 MenuCursor_GetCenterY(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
BOOL MenuCursor_HasSelection(void);
void Menu_Reset(void);
u8 MenuButton_GetTouchState(MenuButton* button);
void MenuButton_SetTouchState(MenuButton* button, int state);

extern MenuCursor data_0229b38c; /* what gMenuCursor points to */

#endif
