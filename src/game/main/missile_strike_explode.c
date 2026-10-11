#include "types.h"
#include "game/missile_strike_task.h"

void ScreenFlash_Start(int, int, int, int);
void ScreenShake_Start(int script, int frames, int parent);
void Units_DamageAroundTile(int x, int y, int damage, int list);

/* Script step of data_02169088 on impact: screen flash and shake, then 80
 * damage to the units on tile offset list 2 around the target tile. */
void MissileStrike_Explode(MissileStrikeTask *task)
{
    ScreenFlash_Start(4, 0, 20, 0);
    ScreenShake_Start(2, 90, 0);
    Units_DamageAroundTile(task->tileX, task->tileY, 80, 2);
}
