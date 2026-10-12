#include "types.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);
void func_0208ac7c(void);
void func_020b9864(void);
void Task_ResumeByScript(void* script);
extern u8 data_02161510[];

void func_0208a128(void) {
  Sound_PlaySe(0x10009);
  MenuButtons_RemoveSelectable();
  func_0208ac7c();
  func_020b9864();
  Task_ResumeByScript(data_02161510);
}
