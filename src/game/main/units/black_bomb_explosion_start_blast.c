#include "types.h"
#include "game/black_bomb_blast_task.h"
#include "game/task.h"

void BlackBombBlast_Start(int x, int y, int gfxTile, int palette, int damage,
                          Task* parent);

/* Explosion task step: starts the blast at the explosion tile with sprite
 * graphics tile 404 and sprite palette 5, as a child of this task. */
void BlackBombExplosion_StartBlast(BlackBombExplosionTask* task) {
  BlackBombBlast_Start(task->tileX, task->tileY, 404, 5, task->damage,
                       (Task*)task);
}
