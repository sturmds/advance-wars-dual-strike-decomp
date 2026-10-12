#include "types.h"
#include "game/keypad.h"
#include "game/main_loop.h"

#define REG_KEYINPUT (*(volatile u16*)0x04000130)
#define HW_BUTTON_XY_BUF \
  (*(volatile u16*)0x027fffa8) /* X, Y and debug bits, written by the ARM7 */
#define PAD_ALL_MASK 0x2fff
#define RESET_COMBO \
  (PAD_BUTTON_SELECT | PAD_BUTTON_START | PAD_BUTTON_R | PAD_BUTTON_L)

extern MainLoop gMainLoop;

int Link_IsActive(void);
int func_020fcad8(void);
void MainLoop_Shutdown(void);

/* Holding L, R, Start and Select together shuts the game down (unless a link
 * is up). Only the frame where the combo completes counts. */
void MainLoop_CheckResetCombo(void) {
  u16 keys = ((REG_KEYINPUT | HW_BUTTON_XY_BUF) ^ PAD_ALL_MASK) & PAD_ALL_MASK;

  if (Link_IsActive() == 1) {
    return;
  }
  if (func_020fcad8() == 1) {
    return;
  }
  if ((keys & RESET_COMBO) == RESET_COMBO &&
      (gMainLoop.prevKeys & RESET_COMBO) != RESET_COMBO) {
    MainLoop_Shutdown();
  }
  gMainLoop.prevKeys = keys;
}
