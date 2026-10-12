#include "types.h"

int Task_ResumeByScript(const void* script);

extern u8 data_0215b22c[];

int func_0204cd00(void) { return Task_ResumeByScript(data_0215b22c); }
