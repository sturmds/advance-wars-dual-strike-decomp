#include "types.h"

void Sound_StopSubSe(int);
int Task_DestroyByScript(void*);
extern u8 data_ov002_02369790[];

int func_ov002_0235f9b0(void) {
  Sound_StopSubSe(0);
  return Task_DestroyByScript(data_ov002_02369790);
}
