#include "types.h"

void Task_DestroyByScript(void *script);
extern u32 data_02160ed0[];
extern u32 data_02160f00[];
extern u32 data_02160f30[];
extern u32 data_02160f60[];

void RowLockMark_DestroyAll(void)
{
    Task_DestroyByScript(data_02160ed0);
    Task_DestroyByScript(data_02160f00);
    Task_DestroyByScript(data_02160f30);
    Task_DestroyByScript(data_02160f60);
}
