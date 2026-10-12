#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiQueue_SortDescending(int count);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sorts gAiQueue by free cargo slots (0-2), most first. */
void AiQueue_SortBlackBoats(void) {
  int i;
  MapUnit* unit;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    unit = &STATE->units[gAiQueue[i]];
    if (unit->cargo[0] == 0) {
      gAiQueueSortKeys[i] = 2;
    } else if (unit->cargo[1] == 0) {
      gAiQueueSortKeys[i] = 1;
    } else {
      gAiQueueSortKeys[i] = 0;
    }
  }
  AiQueue_SortDescending(i);
}
