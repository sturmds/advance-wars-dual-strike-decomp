#include "types.h"
#include "game/keypad.h"
#include "game/touch.h"
#include "game/win_lose.h"

void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(WinLoseTask *task);
void func_0203e074(WinLoseTask *task);
void func_0203f340(s32 added, s32 total, int popFrame);   /* likely draws the points, like PointsTally_Draw */

/* Task update of the two-side win/lose screen, like PointsTally_Count:
 * after 30 frames moves two points per frame from unk_58 into the total
 * unk_5c, with a tick sound every other frame. A or a touch moves the
 * rest at once. The script resumes when unk_58 is empty. */
void WinLoseScreen_CountPoints(WinLoseTask *task)
{
    func_0203e074(task);
    func_0203f340(task->unk_58, task->unk_5c, -1);
    if (task->unk_58 == 0 && task->unk_4c >= 30) {
        task->unk_4c = 0;
        Task_ResumeScript(task);
        return;
    }
    task->unk_4c++;
    if (task->unk_4c >= 30) {
        task->unk_58 -= 2;
        task->unk_5c += 2;
        while (task->unk_58 < 0) {
            task->unk_58++;
            task->unk_5c--;
        }
        if (task->unk_5c > WIN_LOSE_POINTS_MAX) {
            task->unk_5c = WIN_LOSE_POINTS_MAX;
        }
        if ((task->unk_4c & 1) == 0) {
            Sound_PlaySe(0x1000c);
        }
    }
    if ((gKeypad->pressed & PAD_BUTTON_A) || (gTouch.phase == TOUCH_PRESSED && gTouch.active == 1)) {
        task->unk_5c += task->unk_58;
        task->unk_58 = 0;
        task->unk_4c = 0;
        if (task->unk_5c > WIN_LOSE_POINTS_MAX) {
            task->unk_5c = WIN_LOSE_POINTS_MAX;
        }
        Task_ResumeScript(task);
    }
}
