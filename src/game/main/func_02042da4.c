#include "types.h"

void CoSelect_ApplyPicks(void);
int Mem_Free(int);
extern int data_0229c73c;

int func_02042da4(void) {
  CoSelect_ApplyPicks();
  return Mem_Free(data_0229c73c);
}
