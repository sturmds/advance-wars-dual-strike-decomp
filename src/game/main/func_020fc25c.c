#include "types.h"

extern int data_022a7e68;
void Task_ResumeScript(void* task);
void func_0207d934(void);
void DownloadPlay_StopParent(void);
void func_020fc79c(void);

void func_020fc25c(void* task) {
  switch (data_022a7e68) {
    case 0:
      func_020fc79c();
      Task_ResumeScript(task);
      break;
    case 6:
      DownloadPlay_StopParent();
      func_0207d934();
      break;
  }
}
