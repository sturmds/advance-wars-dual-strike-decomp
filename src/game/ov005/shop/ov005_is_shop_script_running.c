#include "types.h"

/* Set by Ov005_StartEventScriptTask, cleared when the script ends. */
/* also declared in src/game/ov005/func_ov005_02367eac.c and 2 more */
int data_ov005_0236bbb4;
u8 data_ov005_0236bbb8[0x8]; /* 0x8 bytes no code names, before the end */

/* Nonzero while an event script that overlay 5 started still runs. */
int Ov005_IsShopScriptRunning(void) { return data_ov005_0236bbb4; }
