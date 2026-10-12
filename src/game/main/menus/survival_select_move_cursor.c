#include "types.h"
#include "game/keypad.h"
#include "game/page_strip.h"

void* MenuButton_Select(int id);
int SurvivalSelect_GetList(void);

/* Left and right move the strip's selected button by one, within the
 * current list. */
void SurvivalSelect_MoveCursorOnKeys(u8* cursor) {
  int target = *cursor;
  int list = SurvivalSelect_GetList();

  if (gKeypad->repeat & PAD_KEY_LEFT) {
    if (target != 0) {
      target--;
    }
  }
  if (gKeypad->repeat & PAD_KEY_RIGHT) {
    if (target < data_0229c744->lists[list].count - 1) {
      target++;
    }
  }
  if (target != *cursor) {
    MenuButton_Select(target);
  }
}
