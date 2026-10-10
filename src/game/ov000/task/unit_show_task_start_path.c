#include "types.h"
#include "game/unit_show_task.h"

extern u8 data_ov000_022db6cc[];

UnitShowTask *Task_FindByScript(void *script);
void func_ov000_022ae5c0(UnitShowTask *task, u8 *path);

/* Gives the running unit show task a path to walk; does nothing when there is none. */
void UnitShowTask_StartPath(u8 *path)
{
    UnitShowTask *task = Task_FindByScript(data_ov000_022db6cc);

    if (task == NULL) {
        return;
    }
    func_ov000_022ae5c0(task, path);
}
