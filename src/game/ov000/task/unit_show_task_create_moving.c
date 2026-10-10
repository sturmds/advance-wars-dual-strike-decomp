#include "types.h"
#include "game/unit_show_task.h"

UnitShowTask *UnitShowTask_Spawn(int x, int y, int player, int type);
void func_ov000_022ae5c0(UnitShowTask *task, u8 *path);

/* Makes a show task for a unit and gives it a path to walk (16 move steps,
 * as func_ov000_022ae5c0 reads them). NULL when no task could be made. */
UnitShowTask *UnitShowTask_CreateMoving(int x, int y, int player, int type, u8 *path)
{
    UnitShowTask *task = UnitShowTask_Spawn(x, y, player, type);

    if (task == NULL) {
        return NULL;
    }
    func_ov000_022ae5c0(task, path);
    return task;
}
