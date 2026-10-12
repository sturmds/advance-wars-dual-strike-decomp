#include "types.h"
#include "game/ai_unit_action.h"

/* Bubble sort of the first count entries of gAiQueue by their keys in
 * gAiQueueSortKeys, largest key first. */
void AiQueue_SortDescending(int count) {
  int i;
  int j;
  u8 tmp;

  if (count < 2) {
    return;
  }
  for (i = 0; i <= count - 2; i++) {
    for (j = count - 2; j >= i; j--) {
      if (gAiQueueSortKeys[j] < gAiQueueSortKeys[j + 1]) {
        tmp = gAiQueueSortKeys[j];
        gAiQueueSortKeys[j] = gAiQueueSortKeys[j + 1];
        gAiQueueSortKeys[j + 1] = tmp;
        tmp = gAiQueue[j];
        gAiQueue[j] = gAiQueue[j + 1];
        gAiQueue[j + 1] = tmp;
      }
    }
  }
}
