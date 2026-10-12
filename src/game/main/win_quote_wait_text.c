#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"
#include "game/win_quote.h"

BOOL Text_IsPrinting(void);
void Task_ResumeScript(Task* task);
MenuButton* WinQuote_AddCloseButton(void);

/* Step of the win quote screen: draws the COs while the quote prints, then
 * adds the button that closes the screen and moves on. */
void WinQuote_WaitText(Task* task) {
  WinQuote_DrawCos(task);
  if (Text_IsPrinting()) {
    return;
  }
  WinQuote_AddCloseButton();
  Task_ResumeScript(task);
}
