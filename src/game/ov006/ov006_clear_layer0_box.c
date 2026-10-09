#include "types.h"
#include "game/Unk_02088d04.h"

void func_020d796c(void);
void func_01ff8a2c(u16 *map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280 *data_027c0280;

/* Clears the 15x10 tile box at (16, 5) on layer 0. func_020d796c is called
 * without arguments, as in the original. */
void Ov006_ClearLayer0Box(void)
{
    func_020d796c();
    func_01ff8a2c(data_027c0280->layer0Map, 16, 5, 15, 10, 0);
    Gfx_RequestLayerCopy(0);
}
