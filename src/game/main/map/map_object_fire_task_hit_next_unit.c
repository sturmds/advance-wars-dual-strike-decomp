#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object_turn_task.h"
#include "game/unit_target_entry.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern UnitTargetEntry *data_022a6dcc;

void Unit_DamageAtTile(int x, int y, int damage);
void func_020c4018(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step of the map object fire script: damages the unit of the current AI
 * target list entry where it stands, then moves to the next entry. */
void MapObjectFireTask_HitNextUnit(MapObjectFireTask *task)
{
    UnitTargetEntry *list = data_022a6dcc;
    MapUnit *unit = &STATE->units[list[task->nextEntry].unitSlot];

    Unit_DamageAtTile(unit->x, unit->y, list[task->nextEntry].value);
    task->nextEntry++;
    func_020c4018();
}
