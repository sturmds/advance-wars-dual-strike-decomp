#include "types.h"
#include "game/keypad.h"

void func_0208af4c(void);

/* Key step of the Combat result screen: A moves on (func_0208af4c plays the
 * sound, resumes the script and closes button 254). */
void CombatResult_OnKeys(void) {
  if (gKeypad->pressed & PAD_BUTTON_A) {
    func_0208af4c();
  }
}
