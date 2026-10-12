#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/task.h"

void func_020eeddc(int x, int y, Task* parent);
void func_020c7388(int unitIndex);
void func_020c8888(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Like UnitDestroyTask_PlayEffectAndRemove, but always plays effect 0
 * (func_020eeddc), whatever the unit type. */
void UnitDestroyTask_PlayDefaultEffectAndRemove(Task* task) {
  MapUnit* unit = task->unk_4c;

  func_020eeddc(unit->x, unit->y, task);
  func_020c7388(unit - STATE->units);
  func_020c8888();
}
