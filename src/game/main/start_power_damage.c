#include "types.h"
#include "game/power_damage.h"

/* Task system: the second argument is a priority when below 8, else the
 * parent task. */
void *func_02023a0c(const void *desc, void *parent);
void *func_02023a58(const void *desc, void *parent);

extern u8 gPowerDamageTaskDesc[];

void StartPowerDamage(const void *effectDesc, const char *effectA, const char *effectB,
                      int player, int damage, int heal, int weather, BOOL halveFuel,
                      PowerDamageFilter filter, void *parent)
{
    PowerDamageTask *task = func_02023a0c(gPowerDamageTaskDesc, parent);
    PowerEffectTask *effect;

    task->player = player;
    task->damage = damage;
    task->heal = heal;
    task->weather = weather;
    task->halveFuel = halveFuel;
    task->filter = filter;
    effect = func_02023a58(effectDesc, task);
    effect->effectA = effectA;
    effect->effectB = effectB;
}
