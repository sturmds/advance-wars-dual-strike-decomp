#ifndef GAME_POPUP_MENU_H
#define GAME_POPUP_MENU_H

#include "types.h"
#include "game/menu_item.h"

/* The fields of a pop-up menu task (a 0x6c-byte Task created by
 * func_0201e9d8) that the code seen so far reads. */
typedef struct PopupMenuTask {
    u8 pad_00[0x2c];
    const MenuItem *items;             /* item table */
    u8 pad_30[0x51 - 0x30];
    u8 selected;                       /* cursor position */
    u8 pad_52[0x58 - 0x52];
    s16 x;                             /* top-left tile */
    s16 y;
    s8 style;                          /* 0 or 1: picks Task.onDestroy */
    u8 pad_5d[0x6c - 0x5d];
} PopupMenuTask;

/* A pop-up menu pushed on the stack by PopupMenu_PushState, 8 bytes. */
typedef struct PopupMenuSave {
    s8 x;
    s8 y;
    u8 selected;
    s8 style;
    const MenuItem *items;
} PopupMenuSave;

#endif
