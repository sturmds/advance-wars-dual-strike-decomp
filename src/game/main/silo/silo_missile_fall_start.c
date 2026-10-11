#include "types.h"
#include "game/task.h"

void SiloMissile_LoadGfx(int tile, int palette);
void SiloMissileFall_CreateTask(int x, int y, int tile, int palette, int unk44, int unk60, Task *parent);

/* Loads the missile graphics and starts the task of a missile falling onto
 * tile (x, y) under `parent`. unk44 and unk60 go to fields of the new task
 * that SiloMissile_Explode reads. */
void SiloMissileFall_Start(int x, int y, int unk44, int unk60, Task *parent)
{
    SiloMissile_LoadGfx(404, 5);
    SiloMissileFall_CreateTask(x, y, 404, 5, unk44, unk60, parent);
}
