#include "types.h"
#include "game/task.h"

void SiloMissile_LoadGfx(int tile, int palette);
void SiloMissileFall_CreateTask(int x, int y, int tile, int palette, int damage,
                                int effect, Task* parent);

/* Loads the missile graphics and starts the task of a missile falling onto
 * tile (x, y) under `parent`. damage and effect go to fields of the new task
 * that SiloMissile_Explode reads. */
void SiloMissileFall_Start(int x, int y, int damage, int effect, Task* parent) {
  SiloMissile_LoadGfx(404, 5);
  SiloMissileFall_CreateTask(x, y, 404, 5, damage, effect, parent);
}
