#include "types.h"
#include "game/task.h"
#include "game/keypad.h"

void ResultStats_Draw(int offset, Task* task);
void ResultStats_NextPage(void);
void ResultStats_PrevPage(void);
void ResultStats_NextPlayer(void);
void ResultStats_PrevPlayer(void);
void ResultStats_ConfirmExit(void);

/* Task update of the stats page: redraws it and reads the keys. */
void ResultStats_Update(Task* task) {
  ResultStats_Draw(0, task);
  if (gKeypad->pressed & PAD_KEY_LEFT) {
    ResultStats_PrevPlayer();
  }
  if (gKeypad->pressed & PAD_KEY_RIGHT) {
    ResultStats_NextPlayer();
  }
  if (gKeypad->pressed & PAD_KEY_UP) {
    ResultStats_PrevPage();
  }
  if (gKeypad->pressed & PAD_KEY_DOWN) {
    ResultStats_NextPage();
  }
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_START)) {
    ResultStats_ConfirmExit();
  }
}
