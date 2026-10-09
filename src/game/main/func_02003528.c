#include "types.h"

void Dma_StopChannel1(void);
void func_020045f4(void (*callback)(void));

void func_02003528(void)
{
    func_020045f4(Dma_StopChannel1);
}
