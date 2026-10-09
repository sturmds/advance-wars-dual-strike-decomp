#include "types.h"

void func_0210f5c0(const void *src, void *dst, u32 size);   /* copies bytes */
void func_0210f458(const void *src, void *dst, u32 size);   /* copies 16-bit units */
void func_0210f488(const void *src, void *dst, u32 size);   /* copies 32-bit units */

/* Copies size bytes in units of unitBits (8, 16 or 32) bits.
 * Any other value copies bytes. */
void Mem_CopyUnits(const void *src, void *dst, u32 size, u32 unitBits)
{
    switch (unitBits) {
    default:
    case 8:
        func_0210f5c0(src, dst, size);
        break;
    case 16:
        func_0210f458(src, dst, size);
        break;
    case 32:
        func_0210f488(src, dst, size);
        break;
    }
}
