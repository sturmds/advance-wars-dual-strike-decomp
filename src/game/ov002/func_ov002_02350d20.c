#include "types.h"

void Battle_ResetScreen(void);
int func_ov002_0235f9b0(void);
void Sound_ReloadResidentData(void);

void func_ov002_02350d20(void) {
  Battle_ResetScreen();
  func_ov002_0235f9b0();
  Sound_ReloadResidentData();
}
