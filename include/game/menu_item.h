#ifndef GAME_MENU_ITEM_H
#define GAME_MENU_ITEM_H

#include "types.h"

/* One item of a pop-up menu table, 0x20 bytes. PopupMenu_Create builds the
 * menu from a table: it calls getFlags on each item and lists the item
 * unless bit 0 (MENU_ITEM_HIDDEN) is set. An item with unk_00 ==
 * MENU_ITEM_END ends the table. */
typedef struct MenuItem {
    u8 unk_00;
    u8 pad_01[3];
    s8 (*getFlags)(void);              /* MENU_ITEM_* */
    u32 unk_08;
    void *unk_0c;
    u32 unk_10;
    void *unk_14;
    void *unk_18;
    u32 textId;                        /* label, passed to Text_Get */
} MenuItem;

#define MENU_ITEM_HIDDEN 1
#define MENU_ITEM_GREYED 2 /* guess: printed in the other text colour, and confirming plays another sound */
#define MENU_ITEM_END    0xff

/* Battle map menu (main 0x02165e8c): Strike, Assault, General, Defense,
 * Auto CO On and Auto CO Off among others. */
extern MenuItem gBattleMenuItems[];

#endif
