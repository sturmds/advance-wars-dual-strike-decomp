#include "types.h"
#include "game/keypad.h"
#include "game/task.h"
#include "game/touch.h"

void Task_ResumeScript(Task *task);
void func_02070e30(int x, int y);

/* Script task: holds the banner at x 128 until A, B or Start is pressed or the
 * touch screen is tapped, then resumes the script. */
void HistoryBanner_WaitForPress(Task *task)
{
    func_02070e30(128, 96);
    if (!(gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START))) {
        if (gTouch.phase != TOUCH_PRESSED) {
            return;
        }
        if (gTouch.active != 1) {
            return;
        }
    }
    Task_ResumeScript(task);
}
