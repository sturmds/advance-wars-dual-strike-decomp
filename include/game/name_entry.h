#ifndef GAME_NAME_ENTRY_H
#define GAME_NAME_ENTRY_H

#include "types.h"
#include "game/name_entry_work.h"
#include "game/Unk_02088d04.h"

/* Tile map of the current screen's layer 0, which the entered text is printed
 * into. A function, not a plain read: the original's call arguments only come
 * out in the right order with it. */
static inline u16* NameEntry_GetTextMap(void) {
  return data_027c0280->layer0Map;
}

#endif
