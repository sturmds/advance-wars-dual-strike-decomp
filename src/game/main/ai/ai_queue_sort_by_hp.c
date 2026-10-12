#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiQueue_SortDescending(int count);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sorts gAiQueue by hp, highest first. A unit that already has a target
 * ranks one above a unit with the same hp and none. */
void AiQueue_SortByHp(void) {
  int i;
  MapUnit* unit;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    unit = &STATE->units[gAiQueue[i]];
    gAiQueueSortKeys[i] = unit->hp * 2;
    if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
      gAiQueueSortKeys[i]++;
    }
  }
  AiQueue_SortDescending(i);
}
