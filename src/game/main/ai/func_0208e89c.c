#include "types.h"
#include "game/ai_unit_action.h"

/* Clears the 0x40 bytes at data_0229e1b0. */
void func_0208d548(void);

void func_0208e89c(void)
{
    data_0229e168 = data_0229e1f0;
    *data_0229e168++ = AI_QUEUE_RUN_ONCE;
    *data_0229e168 = 0;
    data_0229e168 = data_0229e1f0;
    data_0229e170 = func_0208d548;
    *data_021619c8 = AI_STATE_RUN_QUEUE;
}
