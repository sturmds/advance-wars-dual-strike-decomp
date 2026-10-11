#include "types.h"
#include "game/keypad.h"
#include "game/task.h"
#include "game/win_quote.h"

void func_02032abc(int arg); /* defined without parameters; this caller passes 0 */

/* Last step of the win quote screen: draws the COs until A, B or START is
 * pressed, then closes the screen through func_02032abc. */
void WinQuote_WaitKeys(Task *task)
{
    WinQuote_DrawCos(task);
    if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
        func_02032abc(0);
    }
}
