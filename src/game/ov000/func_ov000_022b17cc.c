#include "types.h"
#include "game/menu_button.h"

void Task_ResumeByScript(void* script);
void data_ov000_0234f264(void);

void func_ov000_022b17cc(void) {
  Task_ResumeByScript((void*)data_ov000_0234f264);
  MenuButtons_CloseAll();
}
