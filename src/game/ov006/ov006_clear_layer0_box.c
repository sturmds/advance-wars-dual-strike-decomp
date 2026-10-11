#include "types.h"
#include "game/Unk_02088d04.h"

int func_020d796c(void);
void Bg_FillRect(u16 *map, int x, int y, int width, int height, int tile);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280 *data_027c0280;

/* Clears the 15x10 tile box at (16, 5) on layer 0. */
void Ov006_ClearLayer0Box(void)
{
    func_020d796c();
    Bg_FillRect(data_027c0280->layer0Map, 16, 5, 15, 10, 0);
    Gfx_RequestLayerCopy(0);
}
