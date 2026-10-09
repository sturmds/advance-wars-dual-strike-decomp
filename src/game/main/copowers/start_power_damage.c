#include "types.h"
#include "game/power_damage.h"

/* Task system: the second argument is a priority when below 8, else the
 * parent task. */
void *Task_CreateChild(const void *desc, void *parent);
void *Task_Create(const void *desc, void *parent);

extern u8 gPowerDamageTaskDesc[];

void StartPowerDamage(const void *effectDesc, const char *effectA, const char *effectB,
                      int player, int damage, int heal, int weather, BOOL halveFuel,
                      PowerDamageFilter filter, void *parent)
{
    PowerDamageTask *task = Task_CreateChild(gPowerDamageTaskDesc, parent);
    PowerEffectTask *effect;

    task->player = player;
    task->damage = damage;
    task->heal = heal;
    task->weather = weather;
    task->halveFuel = halveFuel;
    task->filter = filter;
    effect = Task_Create(effectDesc, task);
    effect->effectA = effectA;
    effect->effectB = effectB;
}
