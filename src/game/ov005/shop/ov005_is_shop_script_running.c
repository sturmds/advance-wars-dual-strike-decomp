#include "types.h"

/* Set by Ov005_StartEventScriptTask, cleared when the script ends. */
/* also declared in src/game/ov005/func_ov005_02367eac.c and 2 more */
int data_ov005_0236bbb4;

/* Nonzero while an event script that overlay 5 started still runs. */
int Ov005_IsShopScriptRunning(void) { return data_ov005_0236bbb4; }
