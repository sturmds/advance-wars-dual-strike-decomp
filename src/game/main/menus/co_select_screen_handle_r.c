#include "types.h"
#include "game/co_select_screen.h"

void Sound_PlaySe(int id);
void func_02042b3c(void);

/* R button: buzzes on an empty cell, otherwise plays the accept sound and
 * starts a task (func_02042b3c). */
void CoSelectScreen_HandleR(void) {
  u8* column = &data_0229c73c->coGrid[(data_0229c73c->cursorColumn - 1) * 2];

  if (column[data_0229c73c->cursorRow] == 0) {
    Sound_PlaySe(0x10009);
    return;
  }
  Sound_PlaySe(0x10006);
  func_02042b3c();
}
