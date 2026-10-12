/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/sprite_anim.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
u8* func_021016e4(void);
int NameEntry_GetTextLength(NameEntry* entry);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

void NameEntry_PlaceCaret(NameEntry* entry) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  s16 maxX;
  s16 x;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  maxX = (entry->maxLength - 1) * 8 + 4;
  if (entry->maxLength == 8) {
    x = Text_GetPrintWidth(func_021016e4()) + 4;
  } else {
    x = NameEntry_GetTextLength(entry) * 8 + 4;
  }
  if (x > maxX) {
    x = maxX;
  }
  if (entry->maxLength == 16) {
    x -= 32;
  }
  entry->caret->tileBase = 0xb380;
  SpriteAnim_Draw(entry->caret, x + 96, 24);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
