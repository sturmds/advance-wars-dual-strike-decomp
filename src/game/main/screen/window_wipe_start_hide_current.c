#include "types.h"
#include "sdk/gx.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/window_wipe.h"

int Screen_GetNibble(void);
void Screen_SetNibble(int value);
int func_02007710(int screens);
void Screen_EnableEmptyWindows(void);
void DmaTask_Start(const void *src, volatile void *dest, int unk34, int unk36, void (*callback)(void));
void WindowWipe_BuildCenterOut(void);
void Task_Destroy(Task *task);

/* Script step: like WindowWipe_StartHide, but for the current screen only.
 * Does nothing but end the task if the screen is hidden already. Otherwise it
 * marks the screen hidden and starts the H-blank DMA that feeds WIN0H from
 * WindowWipe_BuildCenterOut's table. */
void WindowWipe_StartHideCurrent(Task *task)
{
    if (Screen_GetNibble() == 1) {
        Task_Destroy(task);
        return;
    }
    Screen_SetNibble(1);
    func_02007710(0);
    task->unk_64 = 1;
    Screen_EnableEmptyWindows();
    data_0216e29c = 0;
    data_0216e2a8 = 0;
    DmaTask_Start(gWipeWin0hTable, data_027c027c == data_027c0288.unk_00 ? REG_WIN0H : REG_DB_WIN0H, 1, 0xa240,
                  WindowWipe_BuildCenterOut);
}
