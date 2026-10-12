#include "types.h"

int WM_Enable(void (*callback)(void*));
void func_020fc6c4(void* arg);

BOOL func_020fc698(void) { return WM_Enable(func_020fc6c4) == 2; }
