#include "types.h"

int SND_GetCurrentCommandTag(void);
void SND_FlushCommand(int);
int SND_WaitForCommandProc(int);

int func_021069a8(void) {
  int t0 = SND_GetCurrentCommandTag();
  SND_FlushCommand(1);
  return SND_WaitForCommandProc(t0);
}
