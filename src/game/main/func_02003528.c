#include "types.h"

void Dma_StopChannel1(void);
void CallbackList_Add(void (*callback)(void));

void func_02003528(void) { CallbackList_Add(Dma_StopChannel1); }
