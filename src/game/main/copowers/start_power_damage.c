#include "types.h"
#include "game/task.h"
#include "game/power_damage.h"

extern const TaskScriptCmd gPowerDamageTaskDesc[];

void StartPowerDamage(const void* effectDesc, const char* effectA,
                      const char* effectB, int player, int damage, int heal,
                      int weather, BOOL halveFuel, PowerDamageFilter filter,
                      void* parent) {
  PowerDamageTask* task =
      (PowerDamageTask*)Task_CreateChild(gPowerDamageTaskDesc, parent);
  PowerEffectTask* effect;

  task->player = player;
  task->damage = damage;
  task->heal = heal;
  task->weather = weather;
  task->halveFuel = halveFuel;
  task->filter = filter;
  effect = (PowerEffectTask*)Task_Create(effectDesc, task);
  effect->effectA = effectA;
  effect->effectB = effectB;
}
