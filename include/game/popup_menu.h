#ifndef GAME_POPUP_MENU_H
#define GAME_POPUP_MENU_H

#include "types.h"
#include "game/menu_item.h"

/* The fields of a pop-up menu task (a 0x6c-byte Task created by
 * PopupMenu_Create) that the code seen so far reads. */
typedef struct PopupMenuTask {
  u8 pad_00[0x2c];
  const MenuItem* items; /* item table */
  s8 flags[10];          /* getFlags result of each listed item */
  u8 itemIndex[10];      /* index in items of each listed item */
  u8 itemIcons[10]; /* per item: index into data_0215875c, or 0xff for none */
  u8 iconSlot; /* PopupMenu_SetItemIcon stores a new icon at itemIcons[iconSlot] */
  u8 itemTotal; /* items in the table, listed or not */
  s8 count;     /* listed items, those without MENU_ITEM_HIDDEN */
  u8 selected;  /* cursor position */
  u8 width;     /* widest label, in tiles */
  u8 pad_53[0x58 - 0x53];
  s16 x; /* top-left tile */
  s16 y;
  s8 style; /* 0 or 1: picks Task.onDestroy */
  u8 pad_5d[0x6c - 0x5d];
} PopupMenuTask;

/* One stack entry written by PopupMenu_PushState, 8 bytes. */
typedef struct PopupMenuSave {
  s8 x;
  s8 y;
  u8 selected;
  s8 style;
  const MenuItem* items;
} PopupMenuSave;

#endif
