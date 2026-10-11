#include "types.h"
#include "game/keypad.h"

void func_0203d09c(void);
void ResultStats_ClosePage(void);

/* Script task step on a stats page: runs func_0203d09c every frame and closes
 * the page when A, B or Start is pressed. */
void ResultStats_WaitForClose(void)
{
    func_0203d09c();
    if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
        ResultStats_ClosePage();
    }
}
