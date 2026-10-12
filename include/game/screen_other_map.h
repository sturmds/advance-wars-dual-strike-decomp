#ifndef GAME_SCREEN_OTHER_MAP_H
#define GAME_SCREEN_OTHER_MAP_H

#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

/* unk_30 of the other screen's map state (the screens are indexed 0 and 1,
 * data_027c0284 is the current one). */
static inline u32 OtherMap_GetUnk30(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  u32 value;

  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
  value = data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_30;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return value;
}

#endif
