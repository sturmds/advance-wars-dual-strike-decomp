#include "types.h"
#include "game/power_damage.h"
#include "game/task.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/co_data.h"
#include "game/script_slot.h"
#include "game/screen_other_map.h"

void Task_ResumeScript(Task* task);
s16 ScriptSlot_Find(const ScriptCommand* script);
BOOL Player_IsInGame(u8 player);
void Unit_DamageAtTile(int x, int y, int damage);
void Unit_HalveFuelAtTile(int x, int y);
void PowerHitEffect_Start(int x, int y, int player);

extern ScriptCommand gMapScrollScript[];

/* Script step of gPowerDamageTaskDesc: every POWER_DAMAGE_HIT_DELAY + 1
 * frames, while no map scroll script runs, damages the next unit of a
 * player on another team that passes the power's filter, halves its fuel
 * for Drake's powers and passes its tile to PowerHitEffect_Start (probably the hit
 * effect, or a scroll to the tile). gMapScrollScript is the map scroll script. Carried units
 * and Oozium-class units are skipped. Resumes the script once every player
 * slot has been walked, or at once when the power does no damage. */
void PowerDamage_HitNextEnemy(PowerDamageTask* task) {
  int slot;
  int i;
  MapUnit* unit;

  if (task->damage == 0) {
    Task_ResumeScript((Task*)task);
    return;
  }
  task->delay++;
  if (ScriptSlot_Find(gMapScrollScript) != -1)
    return;
  if (task->delay > POWER_DAMAGE_HIT_DELAY)
    task->delay = 0;
  else
    return;

  for (slot = task->playerSlot;; slot++) {
    if (slot >= 5) { /* past player slot 4 */
      Task_ResumeScript((Task*)task);
      return;
    }
    if (Player_IsInGame(slot) &&
        MAP_STATE->players[slot].team != MAP_STATE->players[task->player].team)
      break;
  }
  task->playerSlot = slot;

  for (i = task->unitIndex; i < POWER_DAMAGE_UNIT_END; i++) {
    unit = &MAP_STATE->units[i + Ov000_gPlayerFirstUnitSlot[task->playerSlot]];
    if (unit->type == UNIT_NONE)
      continue;
    if (unit->flags & MAP_UNIT_CARRIED)
      continue;
    if (Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_ALL)
      continue;
    if (task->filter != NULL && !task->filter(unit))
      continue;
    Unit_DamageAtTile(unit->x, unit->y, task->damage);
    if (task->halveFuel)
      Unit_HalveFuelAtTile(unit->x, unit->y);
    if (data_027c027c->unk_50[data_027c0284]->second != NULL)
      data_027c027c->unk_50[data_027c0284]->second();
    PowerHitEffect_Start(unit->x, unit->y, task->player);
    break;
  }
  task->unitIndex = i + 1;
  if (task->unitIndex >= POWER_DAMAGE_UNIT_END) {
    task->unitIndex = 1;
    task->playerSlot++;
  }
}
