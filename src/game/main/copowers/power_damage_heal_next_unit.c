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
void PowerHitEffect_Start(int x, int y, int player);

extern ScriptCommand gMapScrollScript[];

/* Script step of gPowerDamageTaskDesc: the healing twin of
 * PowerDamage_HitNextEnemy. Heals the next unit of the power's own player,
 * capped at full hp (100), one unit per hit, then resumes the script and
 * rewinds unitIndex and playerSlot for the next step. */
void PowerDamage_HealNextUnit(PowerDamageTask* task) {
  int i;
  MapUnit* unit;

  if (task->heal == 0) {
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

  if (task->player >= 5) {
    Task_ResumeScript((Task*)task);
    return;
  }

  for (i = task->unitIndex; i < POWER_DAMAGE_UNIT_END; i++) {
    unit = &MAP_STATE->units[i + Ov000_gPlayerFirstUnitSlot[task->player]];
    if (unit->type == UNIT_NONE)
      continue;
    if (unit->flags & MAP_UNIT_CARRIED)
      continue;
    if (Ov000_gUnitStats[unit->type].unitClass == UNIT_CLASS_ALL)
      continue;
    if (unit->hp > 100 - task->heal)
      unit->hp = 100;
    else
      unit->hp += task->heal;
    if (data_027c027c->unk_50[data_027c0284]->second != NULL)
      data_027c027c->unk_50[data_027c0284]->second();
    PowerHitEffect_Start(unit->x, unit->y, task->player);
    break;
  }
  task->unitIndex = i + 1;
  if (task->unitIndex >= POWER_DAMAGE_UNIT_END) {
    Task_ResumeScript((Task*)task);
    task->unitIndex = 1;
    task->playerSlot = 0;
  }
}
