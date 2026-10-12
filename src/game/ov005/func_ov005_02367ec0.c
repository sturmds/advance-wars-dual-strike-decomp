#include "types.h"

extern int data_ov005_0236bbb4;
void EventScript_Start(int, void*, int);

void func_ov005_02367ec0(int a0) {
  data_ov005_0236bbb4 = 1;
  EventScript_Start(a0, &data_ov005_0236bbb4, 1);
}
