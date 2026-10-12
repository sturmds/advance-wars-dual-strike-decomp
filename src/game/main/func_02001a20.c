#include "types.h"

void Player_SetCo(int slot, int which, int coId);

void func_02001a20(int a0, int a1, int a2) { Player_SetCo(a0, a1, a2 & 127); }
