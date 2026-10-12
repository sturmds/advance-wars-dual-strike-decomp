#include "types.h"
#include "game/co_exp.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

void ResultScreen_Confirm(MenuButton* button);
void CoExp_DrawScreen(CoExp* exp, int a, int b, int c);

extern MenuState gMenuState;

/* Task step: redraws the CO cells, and on A, B or Start calls
 * ResultScreen_Confirm. */
void CoExp_WaitForConfirm(void) {
  CoExp_DrawScreen(
      (CoExp*)((u8*)&gMenuState + (u32) & ((MenuState*)0)->coExp.exp), 0, 0,
      -1);
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
    ResultScreen_Confirm(NULL);
  }
}
