#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02290118.h"
#include "game/event_script.h"
#include "game/task.h"

extern Unk_0201d320 data_022977f4[];
extern EventSlotHold data_022977f8[];
extern const TaskScriptCmd data_02158548[];
extern u8 data_02297c2e[];
extern u8 data_02297be0[];

void func_0201d7d0(void);
void EventHold_ReleaseOnPaletteBufferTaskEnd(EventScriptSlot *slot);

/* Event script command: starts the task data_02158548 in task list 3 with the palette buffer
 * data_02297be0, the function that uploads it (func_0201d7d0), the current palette slot and
 * data_02290118.unk_00, then holds the script until that task ends. Ends the slot's
 * run for this frame. */
BOOL EventCmd_StartPaletteBufferTask(s16 index)
{
    Task *task = Task_Create(data_02158548, TASK_LIST(3));

    /* a word over unk_34 and unk_36, which other task scripts use as halfwords */
    *(void **)&task->unk_34 = data_02297c2e;
    task->unk_38 = data_02297be0;
    task->unk_3c = func_0201d7d0;
    task->unk_40 = data_022977a4.unk_00 << 12;
    task->waitFrames = data_02290118.unk_00;
    data_022977f8[index].hold = (u32)EventHold_ReleaseOnPaletteBufferTaskEnd;
    data_022977f4[index].unk_00++;
    return 0;
}
