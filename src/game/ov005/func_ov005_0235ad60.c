#include "types.h"
#include "game/Unk_ov005_0235abfc.h"

/* Returns 1 if a task in the entry list (ended by id 0) has unk_30 == 1. */
int func_ov005_0235ad60(int work, Unk_ov005_0235abfc* entry) {
  int found = 0;

  for (;;) {
    if (entry->id == 0) {
      break;
    }
    if (entry->task->unk_30 == 1) {
      found = 1;
      break;
    }
    entry++;
  }
  return found;
}
