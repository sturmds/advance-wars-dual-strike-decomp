#include "types.h"

void func_0210f488(const void *src, void *dst, u32 size);   /* SDK MI_CpuCopy32 */

/* Copies size bytes in 32-bit units. */
void Mem_Copy32(const void *src, void *dst, u32 size)
{
    func_0210f488(src, dst, size);
}
