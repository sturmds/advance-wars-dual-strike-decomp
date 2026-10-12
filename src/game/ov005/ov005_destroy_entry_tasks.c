#include "types.h"
#include "game/Unk_ov005_0235abfc.h"

void Task_Destroy(Ov005Task* task);

/* Destroys the task of each entry up to the first one with id 0. */
void Ov005_DestroyEntryTasks(int work, Unk_ov005_0235abfc* entry) {
  Ov005Task* noTask = NULL;

  for (;;) {
    if (entry->id == 0) {
      return;
    }
    if (entry->task != NULL) {
      Task_Destroy(entry->task);
    }
    entry->task = noTask;
    entry++;
  }
}
