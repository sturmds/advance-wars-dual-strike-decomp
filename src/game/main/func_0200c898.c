#include "types.h"

extern u32 data_027c0518;
extern u32 data_027c051c;
extern volatile u32 data_027c0520[][4];
extern volatile u32 data_027c0524[][4];
extern volatile u32 data_027c0528[][4];
extern volatile u8 data_027c052c[][16];

/* Not matching: the original reloads the index data_027c051c for each
 * store (five loads) and the compiler shares them. */
void func_0200c898(u32 a, u32 b, u32 size)
{
    data_027c0520[data_027c051c][0] = a;
    data_027c0524[data_027c051c][0] = b;
    data_027c0528[data_027c051c][0] = size;
    data_027c052c[data_027c051c][0] = 4;
    data_027c0518 += size;
    data_027c051c++;
}
