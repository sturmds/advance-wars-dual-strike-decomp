#include "types.h"

typedef struct ShowEntryTask {
  u8 pad_00[0x64];
  s16 progress; /* 20 per frame; the script resumes at 640 (32 frames) */
} ShowEntryTask;

void Task_ResumeScript(ShowEntryTask* task);

void ShowEntryTask_Update(ShowEntryTask* task) {
  if (task->progress >= 640) {
    Task_ResumeScript(task);
  }
  task->progress += 20;
}
