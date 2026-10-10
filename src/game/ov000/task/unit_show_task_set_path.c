#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov000_022db768.h"
#include "game/unit_show_task.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern int data_0229eaec;

void func_020f5888(int x, int y);
void func_020b98e8(void);
int ShowEntry_Finish(Unk_ov000_022db768_Owner *owner);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Copies a path into the task, sets unk_35 to 4 and scrolls the camera
 * to the unit's tile unless unk_18 is 21 with data_0229eaec set. The odd
 * shape of the scroll test (a flag set to 0, then 1, ORed with a second test)
 * is needed to match. */
void UnitShowTask_SetPath(UnitShowTask *task, const u8 *path)
{
    const s8 *steps = (const s8 *)path;
    int i;
    int scroll;

    for (i = 0; i < UNIT_SHOW_PATH_LENGTH; i++) {
        task->script[i] = *steps++;
    }
    scroll = 0;
    task->scriptPos = scroll;
    task->unk_35 = 4;
    if (data_0229eaec == 0) {
        scroll = 1;
    }
    if (scroll | (data_0229eaec != 0 ? GAME->unk_18 != 21 : 0)) {
        func_020f5888((s16)(task->x / 16), (s16)(task->y / 16));
    }
    func_020b98e8();
    ShowEntry_Finish((Unk_ov000_022db768_Owner *)task);
}
