#include "types.h"
#include "game/covering_fire_task.h"
#include "game/strike_target.h"
#include "game/Unk_02080440.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Script step of gCoveringFireScript: picks the site that costs the enemy
 * the most funds for the current player and stores it as the target. */
void CoveringFire_PickFundsSite(CoveringFireTask* task) {
  s16 site[2];

  Strike_FindBestSite(site, STATE->currentPlayer, 0, STRIKE_MODE_FUNDS);
  task->targetX = site[0];
  task->targetY = site[1];
}
