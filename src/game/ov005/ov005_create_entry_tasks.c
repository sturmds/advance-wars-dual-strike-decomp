#include "types.h"
#include "game/Unk_ov005_0235a30c.h"

Ov005Task* Ov005_CreateEntryTask(void* work, int entryIndex, int arg);

/* Creates one task per entry of the first table; entry i gets argument i * 10. */
void Ov005_CreateEntryTasks(Unk_ov005_0235a30c* work) {
  int i = 0;

  for (;;) {
    if (work->entriesA[i].id == 0) {
      return;
    }
    work->entriesA[i].task = Ov005_CreateEntryTask(work, i, i * 10);
    i++;
  }
}
