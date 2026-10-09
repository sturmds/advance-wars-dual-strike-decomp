#include "types.h"

extern u16 data_02297be0[];

void func_01ff8a2c(u16 *map, int x, int y, int width, int height, int tile);

/* Clears the 32 x 6 tile map buffer data_02297be0 (192 entries). */
void EventScript_ClearTextMap(void)
{
    func_01ff8a2c(data_02297be0, 0, 0, 32, 6, 0);
}
