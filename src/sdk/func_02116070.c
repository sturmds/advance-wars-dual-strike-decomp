#include "types.h"

void CARD_WaitBackupAsync(const void *src, void *dst, u32 size);

void func_02116070(const void *src, void *dst, u32 size)
{
    CARD_WaitBackupAsync(src, dst, size);
}
