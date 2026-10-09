#include "types.h"
#include "game/ai_unit_action.h"

void func_0208c560(void);

void func_0208e8fc(void)
{
    data_0229e168 = data_0229e1f0;
    *data_0229e168++ = AI_QUEUE_RUN_ONCE;
    *data_0229e168 = 0;
    data_0229e168 = data_0229e1f0;
    data_0229e170 = func_0208c560;
    *data_021619c8 = AI_STATE_RUN_QUEUE;
}
