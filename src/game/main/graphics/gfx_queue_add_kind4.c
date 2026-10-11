#include "types.h"

extern volatile u32 data_027c0518;
extern volatile u32 data_027c051c;
extern u32 data_027c0520[][4];
extern volatile u32 data_027c0524[][4];
extern volatile u32 data_027c0528[][4];
extern volatile u8 data_027c052c[][16];

/* Adds an entry to a queue of 16-byte entries: a, b, size and type 4.
 * data_027c0518 sums the sizes, data_027c051c counts the entries.
 * Not matching yet (80%): the original reloads the count for every use, as
 * volatile does here, but loads it twice before the first store. With every
 * array volatile the first store comes between those loads (75%); with
 * data_027c0520 not volatile, as here, that store sinks to the end. */
void GfxQueue_AddKind4(u32 a, u32 b, u32 size)
{
    data_027c0520[data_027c051c][0] = a;
    data_027c0524[data_027c051c][0] = b;
    data_027c0528[data_027c051c][0] = size;
    data_027c052c[data_027c051c][0] = 4;
    data_027c0518 += size;
    data_027c051c++;
}
