#include "types.h"
#include "game/co_select_screen.h"

int Co_GetRank(u32 co);
BOOL Skills_AreDisabled(void);
void Sound_PlaySe(int id);
void func_02042b68(void);

/* X button: does nothing while Skills_AreDisabled. Buzzes on an empty cell or
 * a CO with rank 0, otherwise plays the accept sound and starts a task
 * (func_02042b68). */
void CoSelectScreen_HandleX(void) {
  u8* column;
  u8 co;

  if (Skills_AreDisabled() == 1) {
    return;
  }
  column = &data_0229c73c->coGrid[(data_0229c73c->cursorColumn - 1) * 2];
  co = column[data_0229c73c->cursorRow];
  if (co == 0 || Co_GetRank(co) == 0) {
    Sound_PlaySe(0x10009);
    return;
  }
  Sound_PlaySe(0x10006);
  func_02042b68();
}
