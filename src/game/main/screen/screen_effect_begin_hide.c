#include "types.h"
#include "game/screen_effect.h"
#include "game/screen.h"

int func_02007748(void);
int func_02007730(void);
void func_02007710(int a0);
void Task_Destroy(void* task);

BOOL ScreenEffect_BeginHide(Task* task) {
  if (func_02007748() == 1) {
    if (func_02007730() == 1) {
      Task_Destroy(task);
      return 0;
    }
    func_02007710(3);
  } else if (func_02007730() == 1) {
    func_02007710(2);
  } else {
    func_02007710(1);
  }
  Screen_SetBothNibbles(1);
  return 1;
}
