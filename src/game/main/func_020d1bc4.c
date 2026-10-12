#include "types.h"

extern s16 gEventScriptOwnsInput;

int func_020d1bc4(void) { return (gEventScriptOwnsInput != 0) ? 1 : 0; }
