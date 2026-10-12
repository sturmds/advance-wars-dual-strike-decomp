#include "types.h"

void Task_ResumeByScript(void* script);
void data_ov000_0234f264(void);
void MenuButtons_CloseAll(void);

void func_ov000_022b17cc(void) {
  Task_ResumeByScript((void*)data_ov000_0234f264);
  MenuButtons_CloseAll();
}
