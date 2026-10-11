#include "types.h"

extern u16 data_0229da64;
void Task_ForEachByScript(void *, void *, void *, int);
extern u8 data_0216050c[];
void LinkReaction_ClearOwnDestroyCallback(void);

void func_02080844(void)
{
    data_0229da64 = 200;
    Task_ForEachByScript(data_0216050c, (void *)LinkReaction_ClearOwnDestroyCallback, &data_0229da64, 200);
}
