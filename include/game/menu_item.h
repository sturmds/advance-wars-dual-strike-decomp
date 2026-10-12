#ifndef GAME_MENU_ITEM_H
#define GAME_MENU_ITEM_H

#include "types.h"

/* One item of a pop-up menu table, 0x20 bytes. PopupMenu_Create builds the
 * menu from a table: it calls getFlags on each item and lists the item
 * unless bit 0 (MENU_ITEM_HIDDEN) is set. An item with unk_00 ==
 * MENU_ITEM_END ends the table. */
/* Called with the item's index in the table, the button id and the
 * item's MENU_ITEM_* flags. */
typedef void (*MenuItemHandler)(s8 index, s8 button, s8 flags);

typedef struct MenuItem {
  u8 unk_00; /* id passed to MapScript_RunList10 on confirm */
  u8 pad_01[3];
  s8 (*getFlags)(void);     /* MENU_ITEM_* */
  MenuItemHandler onUpdate; /* every frame (PopupMenuButton_Update) */
  MenuItemHandler onSelect; /* cursor moved onto it (PopupMenuButton_Select) */
  u32 unk_10;
  MenuItemHandler onConfirm; /* PopupMenuButton_Confirm */
  MenuItemHandler onCancel;  /* PopupMenuButton_Cancel */
  u32 textId;                /* label, passed to Text_Get */
} MenuItem;

#define MENU_ITEM_HIDDEN 1
#define MENU_ITEM_GREYED \
  2 /* printed in the other text colour; confirming plays another sound,
                            * UnitMenu_Fire does nothing and UnitMenu_Send says why it cannot send */
#define MENU_ITEM_END 0xff

/* Battle map menu (main 0x02165e8c): Strike, Assault, General, Defense,
 * Auto CO On and Auto CO Off among others. */
extern MenuItem gBattleMenuItems[];

/* Unit command menu (main 0x0216600c): Wait, Fire, Capture and the others
 * of UnitMenu_Open. */
extern MenuItem gUnitMenuItems[];

#endif
