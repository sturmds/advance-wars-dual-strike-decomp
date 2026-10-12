#include "types.h"

void DmaTasks_Destroy(void);
int Task_ResumeScript(int);

int func_02005a0c(int a0) {
  DmaTasks_Destroy();
  return Task_ResumeScript(a0);
}
